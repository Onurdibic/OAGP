import os
import json
import torch
import numpy as np
from PIL import Image
import torch.nn.functional as F
from torch.utils.data import Dataset, DataLoader
from torchvision import transforms
import matplotlib.pyplot as plt
import torch.nn as nn
import segmentation_models_pytorch as smp

# Cihaz kontrolü
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print(f"Kullanilan Cihaz: {device}")

# Parametreler
BATCH_SIZE = 8
LEARNING_RATE = 1e-4
NUM_EPOCHS = 25
IMG_SIZE = (640, 360)

# Dosya yolları
TRAIN_ANNOTATIONS_FILE = "C:/Users/T_rab/OneDrive/Desktop/archive/train/annotations/bdd100k_labels_images_train.json"
VAL_ANNOTATIONS_FILE = "C:/Users/T_rab/OneDrive/Desktop/archive/val/annotations/bdd100k_labels_images_val.json"
TRAIN_IMG_DIR = "C:/Users/T_rab/OneDrive/Desktop/archive/train/images"
VAL_IMG_DIR = "C:/Users/T_rab/OneDrive/Desktop/archive/val/images"
TRAIN_MASK_DIR = "C:/Users/T_rab/OneDrive/Desktop/archive/train/merged_masks"
VAL_MASK_DIR = "C:/Users/T_rab/OneDrive/Desktop/archive/val/merged_masks"

# Görüntü dönüşümleri
transform = transforms.Compose([
    transforms.Resize((IMG_SIZE[1], IMG_SIZE[0]), interpolation=Image.BILINEAR),  # (height, width)
    transforms.ToTensor()
])

# Kayıp fonksiyonu
class CombinedLoss(nn.Module):
    def __init__(self, weight_dice=0.6, weight_ce=0.4, smooth=1.0):
        super().__init__()
        self.weight_dice = weight_dice
        self.weight_ce = weight_ce
        self.smooth = smooth
        self.ce = nn.CrossEntropyLoss()

    def forward(self, inputs, targets):
        loss_ce = self.ce(inputs, targets)
        inputs_soft = F.softmax(inputs, dim=1)
        targets_one_hot = F.one_hot(targets, num_classes=inputs.shape[1]) \
                             .permute(0,3,1,2).float()
        inter = (inputs_soft * targets_one_hot).sum((2,3))
        union = inputs_soft.sum((2,3)) + targets_one_hot.sum((2,3))
        dice = (2*inter + self.smooth)/(union + self.smooth)
        loss_dice = 1 - dice.mean()
        return self.weight_dice*loss_dice + self.weight_ce*loss_ce

# Dataset
class BDD100KDataset(Dataset):
    def __init__(self, annotations_file, img_dir, mask_dir, transform=None, subset_size=40000):
        self.img_dir = img_dir
        self.mask_dir = mask_dir
        self.transform = transform
        with open(annotations_file, 'r') as f:
            self.data = json.load(f)
        self.data = self.data[:subset_size]

        self.color_to_class = {
            (0, 0, 0): 0,
            (255, 165, 0): 1,
            (0, 128, 255): 2,
            (255, 0, 0): 3,
            (255, 255, 0): 4
        }

    def __len__(self):
        return len(self.data)

    def rgb_to_class(self, mask_rgb):
        mask_np = np.array(mask_rgb)
        class_mask = np.zeros((mask_np.shape[0], mask_np.shape[1]), dtype=np.uint8)
        for color, class_id in self.color_to_class.items():
            matches = np.all(mask_np == color, axis=-1)
            class_mask[matches] = class_id
        return class_mask

    def __getitem__(self, idx):
        img_name = self.data[idx]['name']
        img_path = os.path.join(self.img_dir, img_name)
        mask_name = img_name.replace(".jpg", ".png")
        mask_path = os.path.join(self.mask_dir, mask_name)

        if not os.path.exists(img_path) or not os.path.exists(mask_path):
            return self.__getitem__((idx + 1) % len(self.data))

        img = Image.open(img_path).convert("RGB")
        mask = Image.open(mask_path).convert("RGB").resize((IMG_SIZE[0], IMG_SIZE[1]), resample=Image.NEAREST)

        class_mask = self.rgb_to_class(mask)
        class_mask = torch.from_numpy(class_mask)

        if self.transform:
            img = self.transform(img)

        return img, class_mask.long()

# Veri yükleyici
def get_data_loaders():
    train_ds = BDD100KDataset(TRAIN_ANNOTATIONS_FILE, TRAIN_IMG_DIR, TRAIN_MASK_DIR, transform, 40000)
    val_ds   = BDD100KDataset(VAL_ANNOTATIONS_FILE, VAL_IMG_DIR, VAL_MASK_DIR, transform, 3000)
    return DataLoader(train_ds, batch_size=BATCH_SIZE, shuffle=True, num_workers=4), \
           DataLoader(val_ds, batch_size=BATCH_SIZE, shuffle=False, num_workers=2)

# Model yükle
def load_model():
    model = smp.Unet(
    encoder_name="mobilenet_v2",
    encoder_weights="imagenet",
    classes=5,
    activation=None
    )

    return model.to(device)


# Değerlendirme
def evaluate_model(model, val_loader, criterion, num_classes=5):
    model.eval()
    total_loss = 0.0
    correct_pixels = 0
    total_pixels = 0
    iou_per_class = {i: [] for i in range(num_classes)}

    with torch.no_grad():
        for images, masks in val_loader:
            images = images.to(device)
            masks = masks.to(device)
            outputs = model(images)
            loss = criterion(outputs, masks)
            total_loss += loss.item()
            preds = torch.argmax(outputs, dim=1)
            correct_pixels += torch.sum(preds == masks).item()
            total_pixels += masks.numel()
            for cls in range(num_classes):
                pred_inds = (preds == cls)
                target_inds = (masks == cls)
                intersection = (pred_inds & target_inds).sum().item()
                union = (pred_inds | target_inds).sum().item()
                if union > 0:
                    iou = intersection / union
                    iou_per_class[cls].append(iou)

    avg_val_loss = total_loss / len(val_loader)
    accuracy = correct_pixels / total_pixels
    mean_iou = np.mean([np.mean(iou_per_class[cls]) if iou_per_class[cls] else 0 for cls in range(num_classes)])
    class_iou_avg = {cls: np.mean(iou_per_class[cls]) if iou_per_class[cls] else 0 for cls in iou_per_class}

    return avg_val_loss, accuracy, mean_iou, class_iou_avg

# Eğitim
def train_model(model, train_loader, val_loader):
    optimizer = torch.optim.AdamW(model.parameters(), lr=LEARNING_RATE)
    criterion = CombinedLoss()
    best_foreground_iou = 0.0
    total_images = len(train_loader.dataset)
    processed_images = 0

    for epoch in range(NUM_EPOCHS):
        model.train()
        epoch_loss = 0

        for batch_idx, (images, masks) in enumerate(train_loader):
            images, masks = images.to(device), masks.to(device)

            outputs = model(images)
            loss = criterion(outputs, masks)

            optimizer.zero_grad()
            loss.backward()
            optimizer.step()

            epoch_loss += loss.item()
            processed_images += images.size(0)

            print(f"Epoch [{epoch+1}/{NUM_EPOCHS}], Batch [{batch_idx+1}/{len(train_loader)}], "
                  f"İşlenen: {processed_images}/{total_images*NUM_EPOCHS}, Batch Loss: {loss.item():.4f}", end='\r')

        avg_loss = epoch_loss / len(train_loader)
        print()

        print(f"\nEpoch [{epoch+1}/{NUM_EPOCHS}], Eğitim Kaybı: {avg_loss:.4f}")

        val_loss, accuracy, mean_iou, class_iou_avg = evaluate_model(model, val_loader, criterion)
        print(f"Epoch [{epoch+1}], Doğrulama Kaybı: {val_loss:.4f}, Genel Doğruluk: {accuracy:.4f}, Ortalama IoU: {mean_iou:.4f}")
        for cls, iou_val in class_iou_avg.items():
            print(f"Sınıf {cls} IoU: {iou_val*100:.2f}%")

        # Foreground mean IoU hesapla (class 1-3)
        foreground_classes = [1, 2, 3]
        foreground_ious = [class_iou_avg[cls] for cls in foreground_classes]
        foreground_mean_iou = np.mean(foreground_ious)

        print(f"Epoch [{epoch+1}], Foreground Ortalama IoU (1-2-3): {foreground_mean_iou:.4f}\n")

        # Eğer foreground mean IoU gelişirse, modeli kaydet
        if foreground_mean_iou > best_foreground_iou:
            best_foreground_iou = foreground_mean_iou
            torch.save(model.state_dict(), "best_mobil640.pth")
            print("✅ En iyi foreground IoU ile model kaydedildi!\n")

    print("🎉 Eğitim tamamlandı.")


# Tahmin görselleştirme
def visualize_predictions(model, val_loader):
    model.eval()
    with torch.no_grad():
        for imgs, masks in val_loader:
            imgs, masks = imgs.to(device), masks.to(device)
            preds = model(imgs).argmax(dim=1).cpu().numpy()
            img_np = imgs[0].cpu().permute(1,2,0).numpy()
            mask_np = masks[0].cpu().numpy()
            pred_np = preds[0]

            fig, axs = plt.subplots(1, 3, figsize=(15, 5))
            axs[0].imshow(img_np)
            axs[0].set_title("Giriş Görseli")
            axs[0].axis("off")
            axs[1].imshow(mask_np, cmap="tab20")
            axs[1].set_title("Gerçek Maske")
            axs[1].axis("off")
            axs[2].imshow(pred_np, cmap="tab20")
            axs[2].set_title("Tahmin Maskesi")
            axs[2].axis("off")
            plt.tight_layout()
            plt.show()
            break

# Ana fonksiyon
def main():
    train_loader, val_loader = get_data_loaders()
    model = load_model()
    train_model(model, train_loader, val_loader)
    # visualize_predictions(model, val_loader)

if __name__ == "__main__":
    main()
