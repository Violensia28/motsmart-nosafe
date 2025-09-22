
# Timer Trigger Mode + OLED — Repo‑Ready

## Cara pakai
1) Upload isi folder ini ke branch `manual-lite` (Delete existing files jika perlu) → commit.
2) Actions akan build otomatis → ambil `.bin` & Flash ZIP dari artifacts.

## OLED
- Default: SSD1306 128x64 I2C, alamat **0x3C**, pin **SDA=21, SCL=22**.
- Ubah alamat di `platformio.ini` via `-DOLED_ADDR=0x3D` jika modulmu pakai 0x3D.
- Jika modulmu SH1106, ubah tipe konstruktor di `src/main.cpp` sesuai driver U8g2.

## Wiring modul timer (pakai terminal IN1, G — V2, G)
- **ESP32 TRIG_OUT (GPIO26)** → **PC817 sisi A (+)**
- **ESP32 G** → **PC817 sisi A (−)**
- **IN1 (modul timer)** → **PC817 sisi B (OUT)**
- **G (modul timer)** → **PC817 sisi B (G)**
- **V2 (modul timer)** → **+ supply modul** (5V/12V sesuai modul)
- **G (modul timer)** → **0V modul**

## Peringatan
- Relai kecil pada modul timer **jangan** untuk primer MOT. Gunakan SSR/contactor ber‑rating inrush; modul timer hanya sebagai control.
