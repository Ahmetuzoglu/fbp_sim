import numpy as np
import matplotlib.pyplot as plt

# C++'ın ürettiği 3 farklı veriyi yüklüyoruz
try:
    phantom = np.loadtxt("build/phantom.csv", delimiter=",")
    sinogram = np.loadtxt("build/sinogram.csv", delimiter=",")
    reconstructed = np.loadtxt("build/reconstructed.csv", delimiter=",")
except FileNotFoundError:
    print("Hata: CSV dosyaları bulunamadı! C++ kodunun çalıştığı klasörden emin ol.")
    exit()

fig, axs = plt.subplots(1, 3, figsize=(16, 5))

# 1. Orijinal Nesne (X-Y Ekseni)
axs[0].imshow(phantom, cmap='gray')
axs[0].set_title("Orijinal Nesne (C++'taki Nesne)")
axs[0].set_xlabel("X (Piksel)")
axs[0].set_ylabel("Y (Piksel)")

# 2. Tarama Verisi (Açı-Işın Ekseni - Kıvrılan Sinogram)
axs[1].imshow(sinogram, cmap='viridis', aspect='auto')
axs[1].set_title("Sinogram (Ham Veri)")
axs[1].set_xlabel("Işın (Yelpaze) İndeksi")
axs[1].set_ylabel("Dönüş Açısı (Derece)")

# 3. Geri Çatım (X-Y Ekseni - FBP ile Yeniden Oluşturulan)
axs[2].imshow(reconstructed, cmap='gray')
axs[2].set_title("Filtrelenmiş Geri Çatım (Sonuç)")
axs[2].set_xlabel("X (Piksel)")
axs[2].set_ylabel("Y (Piksel)")
plt.imshow(reconstructed, cmap='gray', vmin=0, vmax=1)
plt.tight_layout()
plt.show()