import argparse
import cv2
import numpy as np
import torch
import time
import os
from queue import Queue
from threading import Thread
from depth_anything_v2.dpt import DepthAnythingV2

# ================= KONFİGÜRASYON =================
class Config:
    FRAME_W, FRAME_H = 256, 256
    INPUT_SIZE = 308

    DEVICE = "cuda" if torch.cuda.is_available() else "cpu"

    # ROI Ayarları (Uzunluk/Yükseklik yarıya indirildi)
    ROI_COUNT = 5
    ROI_W = FRAME_W // ROI_COUNT
    ROI_H = 50          # 100 -> 50 yapıldı (Yarıya indirildi)
    ROI_Y_POS = 110     # Yeni yüksekliğe göre dikey pozisyon optimize edildi

    # Eşik Değerleri (Yeni ROI boyutuna göre yukarı yönlü uyarlandı)
    LOWER_THRESHOLD = 0.45  # 0.40 -> 0.45 yapıldı
    UPPER_THRESHOLD = 0.55  # 0.50 -> 0.55 yapıldı

    # Robot Kinematiği
    WHEEL_RADIUS = 0.085
    TRACK_WIDTH = 0.45
    RPM_CONV = 60 / (2 * np.pi * WHEEL_RADIUS)

    # Hareket Parametreleri
    CRUISE_V = 0.7
    MIN_V = 0.15
    W_LIMIT = 2.5
    AGRESSION_FACTOR = 1.0
    BASE_WEIGHTS = np.array([-1.2, -2.0, 0.0, 2.0, 1.2])
    
    # Yumuşatma ve Zamanlama
    SMOOTH_V = 0.2
    SMOOTH_W = 0.3
    LOCK_DURATION = 3
    PRED_TIME = 0.5
    PRED_STEPS = 10

    PIXELS_PER_METER = 120

# ================= NAVİGASYON MOTORU =================
class DepthNavigation:
    def __init__(self, model_path):
        self.model = self._init_model(model_path)
        self.curr_v = 0.0
        self.curr_w = 0.0
        self.any_obstacle = False
        self.decision_lock_counter = 0
        self.locked_direction = 0
        
        self.vertical_mask = np.linspace(0.0, 1.0, Config.FRAME_H, dtype=np.float32)[:, None]
        self.vertical_mask = np.repeat(self.vertical_mask, Config.FRAME_W, axis=1)

    def _init_model(self, path):
        config = {"encoder": "vits", "features": 64, "out_channels": [48, 96, 192, 384]}
        model = DepthAnythingV2(**config)
        model.load_state_dict(torch.load(path, map_location=Config.DEVICE))
        return model.to(Config.DEVICE).eval()

    def draw_trajectory(self, img, v, w):
        curr_x, curr_y = Config.FRAME_W // 2, Config.FRAME_H
        curr_theta = -np.pi / 2
        dt = Config.PRED_TIME / Config.PRED_STEPS
        points = [(int(curr_x), int(curr_y))]

        for _ in range(Config.PRED_STEPS):
            curr_theta -= w * dt
            curr_x += v * np.cos(curr_theta) * dt * Config.PIXELS_PER_METER
            curr_y += v * np.sin(curr_theta) * dt * Config.PIXELS_PER_METER
            points.append((int(curr_x), int(curr_y)))

        for i in range(len(points) - 1):
            color = (255, 255, 0) if v > 0.4 else (0, 165, 255)
            cv2.line(img, points[i], points[i+1], color, 3)

        # Sadece son segmenti ok olarak çiziyoruz
        color = (255, 255, 0) if v > 0.4 else (0, 165, 255)
        
        # Son iki noktayı al
        p_start = points[-2]
        p_end = points[-1]
        
        # Oku çiz
        cv2.arrowedLine(img, p_start, p_end, color, 3, tipLength=0.3)

    def process_frame(self, frame):
        with torch.no_grad():
            depth = self.model.infer_image(frame, Config.INPUT_SIZE)

        depth = (depth - depth.min()) / (depth.max() - depth.min() + 1e-6)
        depth = cv2.GaussianBlur(depth, (5, 5), 0)

        mask_roi = depth.copy()
        mask_roi[-int(Config.FRAME_H * 0.20):, :] = 1.0
        depth_u8 = (mask_roi * 255).astype(np.uint8)

        roi_scores = []
        for i in range(Config.ROI_COUNT):
            x1 = i * Config.ROI_W
            roi_zone = depth_u8[Config.ROI_Y_POS:Config.ROI_Y_POS + Config.ROI_H, x1:x1 + Config.ROI_W]
            roi_scores.append(np.mean(roi_zone) / 255.0)
        
        return np.array(roi_scores), depth_u8

    def update_control(self, roi_scores):
        obstacle_count = np.sum(roi_scores > Config.LOWER_THRESHOLD)

        if obstacle_count >= 4:
            self.curr_v = 0.0
            self.curr_w = 0.0
            return 0.0, 0.0, ("ACIL DUR", (0, 0, 255))
        
        max_roi_score = np.max(roi_scores)

        if not self.any_obstacle:
            if max_roi_score > Config.UPPER_THRESHOLD:
                self.any_obstacle = True
        else:
            if max_roi_score < Config.LOWER_THRESHOLD:
                self.any_obstacle = False
                self.decision_lock_counter = 0
                self.locked_direction = 0

        if self.any_obstacle:
            target_v = max(Config.MIN_V, 1.0 - (roi_scores[2] * 0.8))
            current_weights = Config.BASE_WEIGHTS.copy()

            if roi_scores[2] > Config.LOWER_THRESHOLD or self.decision_lock_counter > 0:
                if self.decision_lock_counter == 0:
                    left_side = roi_scores[0] + roi_scores[1]
                    right_side = roi_scores[3] + roi_scores[4]
                    self.locked_direction = 1 if left_side < right_side else -1
                    self.decision_lock_counter = Config.LOCK_DURATION

                current_weights[2] = 2.0 * self.locked_direction
                if self.decision_lock_counter > 0:
                    self.decision_lock_counter -= 1

            raw_w = np.sum(roi_scores * current_weights)
            target_w = np.clip(raw_w * Config.AGRESSION_FACTOR, -Config.W_LIMIT, Config.W_LIMIT)
            mode = ("KACINMA", (0, 0, 255))
        else:
            target_v, target_w = Config.CRUISE_V, 0.0
            mode = ("SEYIR", (0, 255, 0))

        self.curr_v = (self.curr_v * (1 - Config.SMOOTH_V)) + (target_v * Config.SMOOTH_V)
        self.curr_w = (self.curr_w * (1 - Config.SMOOTH_W)) + (target_w * Config.SMOOTH_W)

        return self.curr_v, self.curr_w, mode

# ================= THREADLER =================
def camera_thread(cap, frame_queue):
    while True:
        ret, frame = cap.read()
        if not ret:
            frame_queue.put(None)
            break
        frame = cv2.resize(frame, (Config.FRAME_W, Config.FRAME_H))
        frame_queue.put(frame)

def inference_thread(nav, frame_queue, result_queue):
    while True:
        frame = frame_queue.get()
        if frame is None:
            result_queue.put(None)
            break
        roi_scores, depth_u8 = nav.process_frame(frame)
        result_queue.put((frame, roi_scores, depth_u8))

# ================= MAIN LOOP =================
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--source", choices=["cam", "video"], required=True)
    parser.add_argument("--path", type=str, default=None)
    args = parser.parse_args()

    nav = DepthNavigation("checkpoints/depth_anything_v2_vits.pth")
    cap = cv2.VideoCapture(0 if args.source == "cam" else args.path)

    fps_video = cap.get(cv2.CAP_PROP_FPS)
    if fps_video <= 0 or fps_video > 100: 
        fps_video = 30.0

    target_desktop_path = r"C:\Users\T_rab\OneDrive\Desktop"
    if not os.path.exists(target_desktop_path):
        os.makedirs(target_desktop_path, exist_ok=True)

    zaman_damgasi = time.strftime("%Y%m%d_%H%M%S")
    cikis_yolu = os.path.join(target_desktop_path, f"otonom_cikti_{zaman_damgasi}.mp4")

    cikis_genislik = Config.FRAME_W * 2
    cikis_yukseklik = Config.FRAME_H + 120

    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    out = cv2.VideoWriter(cikis_yolu, fourcc, fps_video, (cikis_genislik, cikis_yukseklik))
    print(f"\n[INFO] Video kaydı başladı.\n[YOL] {cikis_yolu}\n")

    frame_queue = Queue(maxsize=2)
    result_queue = Queue(maxsize=2)

    t_cam = Thread(target=camera_thread, args=(cap, frame_queue), daemon=True)
    t_inf = Thread(target=inference_thread, args=(nav, frame_queue, result_queue), daemon=True)
    t_cam.start()
    t_inf.start()

    prev_time, frame_id, fps = time.time(), 0, 0.0

    while True:
        queue_data = result_queue.get()
        if queue_data is None:
            print("[INFO] İşlem tamamlandı.")
            break
            
        frame, roi_scores, depth_u8 = queue_data
        frame_id += 1

        v, w, (mode_text, mode_color) = nav.update_control(roi_scores)

        v_r = max(0.0, v + (w * Config.TRACK_WIDTH / 2))
        v_l = max(0.0, v - (w * Config.TRACK_WIDTH / 2))
        rpm_r, rpm_l = v_r * Config.RPM_CONV, v_l * Config.RPM_CONV

        depth_vis = cv2.cvtColor(depth_u8, cv2.COLOR_GRAY2BGR)

        for i, score in enumerate(roi_scores):
            x1 = i * Config.ROI_W
            box_c = (0, 0, 255) if (nav.any_obstacle and score > Config.LOWER_THRESHOLD) or score > Config.UPPER_THRESHOLD else (0, 255, 0)
            cv2.rectangle(frame, (x1, Config.ROI_Y_POS), (x1 + Config.ROI_W, Config.ROI_Y_POS + Config.ROI_H), box_c, 2)
            cv2.putText(frame, f"{score:.2f}", (x1 + 5, Config.ROI_Y_POS + 15), cv2.FONT_HERSHEY_SIMPLEX, 0.35, box_c, 1)

        nav.draw_trajectory(frame, v, w)

        info = np.zeros((120, Config.FRAME_W * 2, 3), dtype=np.uint8)
        lock_txt = f" | LOCK:{nav.decision_lock_counter}" if nav.decision_lock_counter > 0 else ""
        cv2.putText(info, f"MOD: {mode_text}{lock_txt}", (10, 25), 1, 1, mode_color, 1)
        cv2.putText(info, f"V: {v:.2f} m/s", (10, 50), 1, 1, (255, 255, 255), 1)
        cv2.putText(info, f"W: {w:.2f} rad/s", (Config.FRAME_W + 10, 50), 1, 1, (255, 255, 255), 1)
        cv2.putText(info, f"SOL: {int(rpm_l)} RPM", (10, 85), 1, 1, (0, 255, 255), 2)
        cv2.putText(info, f"SAG: {int(rpm_r)} RPM", (Config.FRAME_W + 10, 85), 1, 1, (0, 255, 255), 2)

        if frame_id % 5 == 0:
            fps = 5.0 / (time.time() - prev_time)
            prev_time = time.time()
        cv2.putText(info, f"FPS: {fps:.1f}", (Config.FRAME_W * 2 - 80, 110), 1, 0.8, (255, 255, 0), 1)

        combined = cv2.vconcat([cv2.hconcat([frame, depth_vis]), info])
        
        out.write(combined)
        cv2.imshow("OTONOM NAVIGASYON", combined)

        if cv2.waitKey(1) & 0xFF == 27:
            print("[INFO] Kullanıcı ESC tuşuna bastı.")
            break

    cap.release()
    out.release()  
    cv2.destroyAllWindows()
    print(f"[BAŞARILI] Video dosyası kaydedildi: {cikis_yolu}")

if __name__ == "__main__":
    main()