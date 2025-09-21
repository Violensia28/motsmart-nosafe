
# MOTSMART Manual-Lite Pack

Fitur ringkas: OLED UI, Rotary Encoder, Trigger tombol & **Pen Switch**, Single/Double pulse, preset ketebalan 0.10/0.12/0.15/0.20/0.25 mm dengan saran ms.

## Tambahkan ke repo
1. Salin folder `src/` dan file `platformio.additions.manual-lite.ini` ke root repo.
2. Append isi `platformio.additions.manual-lite.ini` ke `platformio.ini` (bawah file).
3. Commit & push → GitHub Actions build `manual-lite`.

## Flash (Android ESP32 Flash Tool)
- bootloader.bin @ 0x1000
- partitions.bin @ 0x8000
- boot_app0.bin @ 0xe000
- firmware-manual-lite.bin @ 0x10000

Lihat `PC817_WIRING.md` untuk driver SSR via PC817.
