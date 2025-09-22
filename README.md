
# Manual‑Lite v3 — SSR UI (Repo‑Ready)

## Fitur UI v3
- Mode **SINGLE/DOUBLE** dengan tampilan modern (chip + blink pada field aktif)
- Preset parameter: **Single** (ms), **Double**: **Pre**, **Delay**, **Main**
- **Encoder** (A=32, B=33, BTN=25), **Trigger BTN=27**, **SSR PIN=26**, **Buzzer=19**
- OLED SSD1306 128x64 I2C (**SDA=21, SCL=22, addr 0x3C**)

## Wiring (tanpa istilah kaki; gunakan terminal modul)
### ESP32 → PC817 (sisi input)
- **TRIG_OUT (GPIO26)** → **V1**
- **G (GND)** → **G**

### PC817 (sisi output) → SSR (input)
- **IN1** → **SSR (-)**
- **G**   → **0V (GND supply SSR)**
- **SSR (+)** → **+5V** (atau supply sesuai SSR DC input)

> PC817 output akan **menarik SSR (-) ke G** saat ON. Pastikan SSR kamu tipe **DC input** (contoh 3–32VDC input) dan supply input SSR sesuai spesifikasi.

### SSR (keluaran) → MOT (primer)
- Gunakan SSR **AC output** yang sesuai tegangan/listrik jala‑jala kamu, sambungkan **seri** dengan **AC Live** ke primer MOT (sesuai instruksi SSR). **Ground/N** langsung ke sisi lain primer.
- **Catatan keselamatan**: arus inrush primer MOT **sangat besar**. Pastikan rating SSR **lebih besar** dari arus puncak (inrush) dan gunakan kabel, sekering, serta proteksi yang sesuai.

## Cara Pakai (GitHub Actions)
1. Upload **isi** ZIP ini ke branch `manual-lite` (centang *Delete existing files*).
2. Tab **Actions** → workflow `manual-lite-ci` berjalan otomatis.
3. Ambil artifacts:
   - `firmware-manual-lite/firmware.bin` → flash @ `0x10000`
   - `flash-bundle-manual-lite/flash-bundle-manual-lite-<sha>.zip` → paket lengkap (bootloader/partitions/boot_app0/firmware + README offset)

## Tips
- Jika OLED tidak tampil, cek alamat: ganti `-DOLED_ADDR=0x3D` di `platformio.ini`.
- Jika encoder arah terbalik, tukar **ENC_A/ENC_B** di `build_flags` atau hardware.
- Jika SSR tidak aktif, periksa supply SSR input (+) dan jalur **IN1/G** dari PC817.

## Peringatan
- **JANGAN** menyalakan primer MOT tanpa pelindung yang sesuai (sekering, kabel, isolasi). Ikuti standar keselamatan listrik.
