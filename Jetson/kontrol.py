import cv2
import torch
import numpy as np
from PIL import Image
import torchvision.transforms as transforms
import segmentation_models_pytorch as smp
import time
import os

# ================= KİNEMATİK VE MOTOR KONFİGÜRASYONU =================
class Config:
    FRAME_W, FRAME_H = 640, 360
    
    WHEEL_RADIUS = 0.085  # 8.5 cm
    TRACK_WIDTH = 0.45    # 45 cm
    RPM_CONV = 60 / (2 * np.pi * WHEEL_RADIUS)

    # Hareket Parametreleri
    CRUISE_V = 0.8        
    MIN_V = 0.12          
    W_LIMIT = 2.8         
    
    # P Kontrol Kazancı
    KP = 0.02            
    
    SMOOTH_V = 0.2
    SMOOTH_W = 0.3

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print(f"Kullanılan Cihaz: {device}")

# ================= MODEL VE ÖN İŞLEME =================
def modeli_yukle(model_dosyasi):
    model = smp.Unet(
        encoder_name="mobilenet_v2",
        encoder_weights="imagenet",
        classes=5,
        activation=None
    )
    model.load_state_dict(torch.load(model_dosyasi, map_location=device))
    model.to(device)
    model.eval()
    return model

def frame_onisleme(frame, boyut=(Config.FRAME_W, Config.FRAME_H)):
    image = Image.fromarray(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
    image = image.resize(boyut, Image.BILINEAR)
    transform = transforms.Compose([transforms.ToTensor()])
    return transform(image).unsqueeze(0).to(device), np.array(image)

def segmentasyon_tahmin(model, frame):
    input_tensor, resized_np = frame_onisleme(frame)
    with torch.no_grad():
        output = model(input_tensor)
    predicted_mask = torch.argmax(output, dim=1).squeeze(0).cpu().numpy()
    return predicted_mask, resized_np

renk_haritasi = {
    0: [0, 0, 0],         # Arkaplan
    1: [255, 165, 0],    # Alternatif yol (Turuncu)
    2: [0, 128, 255],    # Sürülebilir alan (Mavi)
    3: [255, 0, 0],      # Şeritler (Kırmızı)
    4: [255, 255, 0]     # Yaya geçidi (Sarı)
}

def renkli_maske_olustur(maske):
    h, w = maske.shape
    seg_image = np.zeros((h, w, 3), dtype=np.uint8)
    for cls_id, renk in renk_haritasi.items():
        seg_image[maske == cls_id] = renk
    return seg_image

def maske_filtrele(maske):
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))
    temiz_maske = maske.copy().astype(np.uint8)
    temiz_maske = cv2.morphologyEx(temiz_maske, cv2.MORPH_OPEN, kernel)
    temiz_maske = cv2.morphologyEx(temiz_maske, cv2.MORPH_CLOSE, kernel)
    return temiz_maske

def yol_ve_serit_merkezi_bul(maske):
    h, w = maske.shape
    roi_baslangic_y = int(h * 0.55)
    
    roi_maske = np.zeros_like(maske)
    roi_maske[roi_baslangic_y:h, :] = maske[roi_baslangic_y:h, :]
    
    # 1. Ana Sürülebilir Yol Alanı (Mavi) Merkez Hesabı
    mavi_maske = (roi_maske == 2).astype(np.uint8) * 255
    M_yol = cv2.moments(mavi_maske)
    
    cx_yol, cy_yol = None, None
    if M_yol["m00"] != 0:
        cx_yol = int(M_yol["m10"] / M_yol["m00"])
        cy_yol = int(M_yol["m01"] / M_yol["m00"])

    # 2. Ham Şerit Maskesi (Kırmızı) ve Koridor Filtreleme
    ham_kirmizi_maske = (roi_maske == 3).astype(np.uint8) * 255
    kernel_genisletme = cv2.getStructuringElement(cv2.MORPH_RECT, (41, 1))
    genisletilmis_yol = cv2.dilate(mavi_maske, kernel_genisletme, iterations=1)
    ana_yol_serit_maskesi = cv2.bitwise_and(ham_kirmizi_maske, genisletilmis_yol)

    # 3. YOLA GÖRE KONUM TABANLI TARAMA
    tarama_y = int(h * 0.70) 
    
    serit_pikselleri_x = np.where(ana_yol_serit_maskesi[tarama_y, :] == 255)[0]
    yol_pikselleri_x = np.where(mavi_maske[tarama_y, :] == 255)[0]
    
    cx_serit, cy_serit = None, tarama_y

    sol_yol_sinir = yol_pikselleri_x[0] if len(yol_pikselleri_x) > 0 else 0
    sag_yol_sinir = yol_pikselleri_x[-1] if len(yol_pikselleri_x) > 0 else w

    if len(serit_pikselleri_x) > 0:
        sol_serit_x = serit_pikselleri_x[0]
        sag_serit_x = serit_pikselleri_x[-1]
        
        if (sag_serit_x - sol_serit_x) > 80:
            cx_serit = int((sol_serit_x + sag_serit_x) / 2)
        else:
            serit_ortasi = int(np.mean(serit_pikselleri_x))
            sol_kenara_mesafe = abs(serit_ortasi - sol_yol_sinir)
            sag_kenara_mesafe = abs(sag_yol_sinir - serit_ortasi)
            
            if sol_kenara_mesafe < sag_kenara_mesafe:
                sol_referans = serit_ortasi
                sag_referans = sag_yol_sinir  
            else:
                sol_referans = sol_yol_sinir  
                sag_referans = serit_ortasi
                
            cx_serit = int((sol_referans + sag_referans) / 2)
            
    else:
        if len(yol_pikselleri_x) > 0:
            cx_serit = int((sol_yol_sinir + sag_yol_sinir) / 2)

    varsayilan_x = int(w / 2)
    varsayilan_y = int(h * 0.82)
    
    arac_alti_roi = ana_yol_serit_maskesi[int(h * 0.85):h, int(w*0.45):int(w*0.55)]
    serit_ihlali = np.sum(arac_alti_roi) > 0

    # Füzyon Kararı
    if cx_yol is not None and cx_serit is not None:
        fused_x = int(cx_yol * 0.5 + cx_serit * 0.5) 
        fused_y = int(cy_yol * 0.5 + cy_serit * 0.5)
        return (fused_x, fused_y), ana_yol_serit_maskesi, roi_baslangic_y, serit_ihlali
    elif cx_yol is not None:
        return (cx_yol, cy_yol), ana_yol_serit_maskesi, roi_baslangic_y, serit_ihlali
    elif cx_serit is not None:
        return (cx_serit, cy_serit), ana_yol_serit_maskesi, roi_baslangic_y, serit_ihlali
    else:
        return (varsayilan_x, varsayilan_y), ana_yol_serit_maskesi, roi_baslangic_y, serit_ihlali


# ================= ANA VİDEO VE NAVİGASYON DÖNGÜSÜ =================
def video_isle(video_yolu, model, cikis_yolu="cikti_video.mp4"):
    video = cv2.VideoCapture(video_yolu)
    if not video.isOpened():
        print("Video açılamadı.")
        return

    fps_video = video.get(cv2.CAP_PROP_FPS)
    if fps_video == 0 or fps_video > 100: fps_video = 30
    
    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    out = cv2.VideoWriter(cikis_yolu, fourcc, fps_video, (Config.FRAME_W, Config.FRAME_H + 120))
    
    curr_v, curr_w = 0.0, 0.0
    prev_time = time.time()

    while True:
        ret, frame = video.read()
        if not ret:
            break

        su_an = time.time()
        dt = su_an - prev_time
        prev_time = su_an

        tahmin_maske, yeniden_boyutlu = segmentasyon_tahmin(model, frame)
        tahmin_maske = maske_filtrele(tahmin_maske)
        renkli_maske = renkli_maske_olustur(tahmin_maske)

        if renkli_maske.shape[:2] != yeniden_boyutlu.shape[:2]:
            renkli_maske = cv2.resize(renkli_maske, (yeniden_boyutlu.shape[1], yeniden_boyutlu.shape[0]))

        saydamlik = 0.5
        karisik = cv2.addWeighted(yeniden_boyutlu, 1 - saydamlik, renkli_maske, saydamlik, 0)
        goruntu_bgr = cv2.cvtColor(karisik, cv2.COLOR_RGB2BGR)

        fused_merkez, ana_serit_maskesi, roi_y, ihlal_durumu = yol_ve_serit_merkezi_bul(tahmin_maske)
        arac_merkez_x = int(Config.FRAME_W / 2)
        
        # Seçtiğimiz ana şerit piksellerini parlak yeşil rengine boyuyoruz
        goruntu_bgr[ana_serit_maskesi == 255] = [0, 255, 0] 

        hata = fused_merkez[0] - arac_merkez_x
        
        target_w = -1.0 * hata * Config.KP
        target_w = np.clip(target_w, -Config.W_LIMIT, Config.W_LIMIT)
        
        hata_oranı = min(1.0, abs(hata) / 200)
        
        if ihlal_durumu:
            target_v = Config.MIN_V
            mode_text, mode_color = ("SERIT IHLALI - FREN", (0, 0, 255))
        else:
            target_v = Config.CRUISE_V - (hata_oranı * (Config.CRUISE_V - Config.MIN_V))
            mode_text, mode_color = ("DUZ SEYIR", (0, 255, 0)) if abs(hata) < 20 else ("SERIT+YOL HATA TAKIBI", (0, 165, 255))
        
        curr_v = (curr_v * (1 - Config.SMOOTH_V)) + (target_v * Config.SMOOTH_V)
        curr_w = (curr_w * (1 - Config.SMOOTH_W)) + (target_w * Config.SMOOTH_W)

        v_r = curr_v + (curr_w * Config.TRACK_WIDTH / 2.0)
        v_l = curr_v - (curr_w * Config.TRACK_WIDTH / 2.0)
        
        v_r = max(0.0, v_r)
        v_l = max(0.0, v_l)
        
        rpm_r = v_r * Config.RPM_CONV
        rpm_l = v_l * Config.RPM_CONV

        # ─── GÖRSELLEŞTİRME DÜZENLEMESİ ───
        # Önceki tüm noktalar, tarama çizgileri ve fazladıklar kaldırıldı.
        # Sadece oklu çubuk kaldı:
        
        # Araç merkezinden (tabandan) hedef merkeze uzanan ince bir çizgi (isteğe bağlı, ok ucuna gitmesi için):
        cv2.line(goruntu_bgr, (arac_merkez_x, Config.FRAME_H), (fused_merkez[0], fused_merkez[1]), (0, 255, 0), 1, cv2.LINE_AA)
        
        # Dönüş yönünü gösteren ana oklu yeşil çubuk:
        # Bu ok, aracın yönelimi ve hata oranına göre hareket eder.
        cv2.arrowedLine(goruntu_bgr, (arac_merkez_x, Config.FRAME_H - 10), (fused_merkez[0], fused_merkez[1] + 10), (0, 255, 0), 4, tipLength=0.1)

        # Alt Bilgi Paneli
        info = np.zeros((120, Config.FRAME_W, 3), dtype=np.uint8)
        cv2.putText(info, f"MOD: {mode_text} | Hata: {hata}px", (10, 25), 1, 1.1, mode_color, 1)
        cv2.putText(info, f"V: {curr_v:.2f} m/s", (10, 55), 1, 1.1, (255, 255, 255), 1)
        cv2.putText(info, f"W: {curr_w:.2f} rad/s", (int(Config.FRAME_W/2) + 10, 55), 1, 1.1, (255, 255, 255), 1)
        cv2.putText(info, f"SOL: {int(rpm_l)} RPM", (10, 90), 1, 1.2, (0, 255, 255), 2)
        cv2.putText(info, f"SAG: {int(rpm_r)} RPM", (int(Config.FRAME_W/2) + 10, 90), 1, 1.2, (0, 255, 255), 2)

        if dt > 0:
            fps = 1.0 / dt
            cv2.putText(info, f"FPS: {fps:.1f}", (Config.FRAME_W - 90, 25), 1, 0.9, (255, 255, 0), 1)

        combined = cv2.vconcat([goruntu_bgr, info])
        
        out.write(combined)
        cv2.imshow("SERIT+YOL ENTEGRELI OTONOM SURUS", combined)

        if cv2.waitKey(1) & 0xFF == 27:
            break

    video.release()
    out.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    model_yolu = "C:/Users/T_rab/OneDrive/Desktop/archive/best_mobil640.pth"
    video_yolu = "C:/Users/T_rab/OneDrive/Desktop/yol6.mp4"
    cildi_yolu = "C:/Users/T_rab/OneDrive/Desktop/yol6_segmentasyon.mp4"

    model = modeli_yukle(model_yolu)
    video_isle(video_yolu, model, cildi_yolu)