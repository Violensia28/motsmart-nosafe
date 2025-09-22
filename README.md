
# Manual-Lite CI — Ultimate Self-Healing — Repo-Ready

## Cara pakai (Upload Format ZIP)
1) Ekstrak ZIP, upload **ISI** folder ini ke branch `manual-lite` (centang *Delete existing files*). 
2) Actions jalan → artifacts tersedia: `firmware.bin` (jika sukses) + Flash ZIP + logs.

## Strategi Self-Healing
- Build#1: pakai proyek yang ada.
- TIER-2: buat proyek OLED minimal (tanpa logika) lalu build.
- TIER-1: buat BareBlink (tanpa lib) lalu build (nyaris pasti berhasil).
- Bundling tetap sukses; kalau tidak ada firmware → `README_NO_FIRMWARE.txt`.

## Wiring (singkat)
- Micro switch → ESP32: GPIO27 ↔ G
- ESP32 → PC817 (input): GPIO26 → V1, G → G
- PC817 (output) → Timer: IN1 → Trigger, G → GND Trigger
- Timer power: USB 5V
- OLED: VCC→3V3, GND→G, SDA→GPIO21, SCL→GPIO22 (0x3C)
