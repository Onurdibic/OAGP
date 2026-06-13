import cv2
import torch
import numpy as np
from PIL import Image
import torchvision.transforms as transforms
import segmentation_models_pytorch as smp
import time
import os

# Cihaz tanımı
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print(f"Kullanılan Cihaz: {device}")

# Model yükleme
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

# Ön işleme
def frame_onisleme(frame, boyut=(640, 360)):
    image = Image.fromarray(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
    image = image.resize(boyut, Image.BILINEAR)
    transform = transforms.Compose([transforms.ToTensor()])
    return transform(image).unsqueeze(0).to(device), np.array(image)

# Segmentasyon tahmini
def segmentasyon_tahmin(model, frame):
    input_tensor, resized_np = frame_onisleme(frame)
    with torch.no_grad():
        output = model(input_tensor)
    predicted_mask = torch.argmax(output, dim=1).squeeze(0).cpu().numpy()
    return predicted_mask, resized_np

# Renk haritası
renk_haritasi = {
    0: [0, 0, 0],
    1: [255, 165, 0],
    2: [0, 128, 255],
    3: [255, 0, 0],
    4: [255, 255, 0]
}

def renkli_maske_olustur(maske):
    h, w = maske.shape
    seg_image = np.zeros((h, w, 3), dtype=np.uint8)
    for cls_id, renk in renk_haritasi.items():
        seg_image[maske == cls_id] = renk
    return seg_image

# Morfolojik filtre
def maske_filtrele(maske):
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))
    temiz_maske = maske.copy().astype(np.uint8)
    temiz_maske = cv2.morphologyEx(temiz_maske, cv2.MORPH_OPEN, kernel)
    temiz_maske = cv2.morphologyEx(temiz_maske, cv2.MORPH_CLOSE, kernel)
    return temiz_maske

# Video işle ve KAYDET
def video_isle(video_yolu, model, cikis_yolu="cikti_video.mp4"):
    video = cv2.VideoCapture(video_yolu)

    if not video.isOpened():
        print("Video açılamadı.")
        return

    genislik = 640
    yukseklik = 360

    fps_video = video.get(cv2.CAP_PROP_FPS)
    if fps_video == 0 or fps_video > 100:
        fps_video = 30

    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    out = cv2.VideoWriter(cikis_yolu, fourcc, fps_video, (genislik, yukseklik))

    print(f"Video işleniyor ve '{cikis_yolu}' adresine kaydediliyor...")

    # ===== FPS İSTATİSTİK =====
    toplam_sure = 0
    frame_sayisi = 0

    while True:
        ret, frame = video.read()
        if not ret:
            break

        baslangic = time.time()

        # Model tahmin
        tahmin_maske, yeniden_boyutlu = segmentasyon_tahmin(model, frame)

        # Filtre
        tahmin_maske = maske_filtrele(tahmin_maske)

        # Renkli maske
        renkli_maske = renkli_maske_olustur(tahmin_maske)

        if renkli_maske.shape[:2] != yeniden_boyutlu.shape[:2]:
            renkli_maske = cv2.resize(
                renkli_maske,
                (yeniden_boyutlu.shape[1], yeniden_boyutlu.shape[0])
            )

        saydamlik = 0.5
        karisik = cv2.addWeighted(yeniden_boyutlu, 1 - saydamlik,
                                 renkli_maske, saydamlik, 0)

        goruntu_bgr = cv2.cvtColor(karisik, cv2.COLOR_RGB2BGR)

        bitis = time.time()
        sure = bitis - baslangic

        # FPS hesap
        toplam_sure += sure
        frame_sayisi += 1

        if sure > 0:
            fps_hesaplanan = 1 / sure
            cv2.putText(goruntu_bgr, f"FPS: {fps_hesaplanan:.2f}", (10, 30),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)

        # yaz
        out.write(goruntu_bgr)
        cv2.imshow("Segmentasyon", goruntu_bgr)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # ===== SONUÇLAR =====
    video.release()
    out.release()
    cv2.destroyAllWindows()

    if toplam_sure > 0:
        ortalama_fps = frame_sayisi / toplam_sure
        print(f"Toplam Frame: {frame_sayisi}")
        print(f"Toplam Süre: {toplam_sure:.2f} sn")
        print(f"Ortalama FPS: {ortalama_fps:.2f}")

    print("İşlem tamamlandı. Video kaydedildi.")

# Ana çalışma
if __name__ == "__main__":
    model_yolu = "C:/Users/T_rab/OneDrive/Desktop/archive/best_mobil640.pth"
    video_yolu = "C:/Users/T_rab/OneDrive/Desktop/yol4.mp4"
    cikti_yolu = "C:/Users/T_rab/OneDrive/Desktop/yol4_segmentasyon.mp4"

    model = modeli_yukle(model_yolu)
    video_isle(video_yolu, model, cikti_yolu)