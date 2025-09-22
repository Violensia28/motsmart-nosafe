
# Micro Switch → ESP32 → PC817 → Timer (OLED) — PULSE 120ms — Repo‑Ready

## Apa yang baru
- PULSE dari ESP32 diperpanjang menjadi **120 ms** agar modul timer lebih mudah mendeteksi trigger.

## Cara pakai
1) Upload isi folder ini ke branch `manual-lite` (centang *Delete existing files*). 
2) Buka tab **Actions** → workflow `manual-lite-ci` jalan otomatis.
3) Ambil artifacts:
   - `firmware-manual-lite/firmware.bin` → flash @ `0x10000`
   - `flash-bundle-manual-lite` → ZIP lengkap untuk ESP32 Flash Tool.

## Wiring ringkas (terminal)
- **Micro switch → ESP32**: Lug 1 → GPIO27, Lug 2 → GND
- **ESP32 → PC817 (input)**: TRIG_OUT (GPIO26) → **V1**, G → **G**
- **PC817 (output) → Timer**: **IN1** → **Trigger**, **G** → **GND Trigger**
- **Power Timer**: USB 5V (atau terminal 6–30V bila tidak via USB)
- **OLED**: VCC→3V3, GND→GND, SDA→GPIO21, SCL→GPIO22 (addr 0x3C)

## Catatan keselamatan
- Relay kecil pada modul timer **jangan** untuk primer MOT. Pakai SSR/contactor ber‑rating inrush; modul timer hanya sebagai kontrol.
- Uji dulu dengan **beban lampu** sebelum menghubungkan beban besar.
