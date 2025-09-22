
# Manual-Lite CI — Self-Healing (PULSE 120ms + OLED) — Repo-Ready

## Cara pakai (Upload Ulang — Format ZIP Siap)
1) Ekstrak ZIP ini, lalu **upload isi folder** ini ke repo GitHub (branch `manual-lite`) via Web UI.
   - Klik **Add files → Upload files**
   - **Drag & drop isi** (bukan ZIP-nya)
   - Centang **Delete existing files** → **Commit**
2) Buka tab **Actions** → workflow `manual-lite-ci` akan berjalan otomatis.
3) Ambil artifacts:
   - `firmware-manual-lite/firmware.bin` → flash @ `0x10000`
   - `flash-bundle-manual-lite` → ZIP lengkap (bootloader/partitions/boot_app0/firmware + README)

## Wiring (terminal, tanpa bahas kaki)
- **Micro switch → ESP32**: Lug 1 → GPIO27, Lug 2 → G (GND)
- **ESP32 → PC817 (input)**: TRIG_OUT (GPIO26) → **V1**, G → **G**
- **PC817 (output) → Timer**: **IN1** → **Trigger**, **G** → **GND Trigger**
- **Power Timer**: USB 5V (atau terminal 6–30V bila tidak via USB)
- **OLED**: VCC→3V3, GND→GND, SDA→GPIO21, SCL→GPIO22 (addr 0x3C)
- **Relay Timer (NO, COM, NC)**: gunakan **NO–COM** untuk kontrol SSR/contactor (disarankan)

## Fitur Self-Healing
Jika build pertama gagal atau file kurang:
- Workflow otomatis **membuat** `platformio.ini` & `src/main.cpp` minimal (PULSE 120ms + OLED)
- **Build ulang** → memastikan `firmware.bin` keluar
- Bundling tetap **sukses**, atau membuat `README_NO_FIRMWARE.txt` untuk diagnosa (job tetap hijau)

## Catatan keselamatan
- Relay kecil modul timer **jangan** untuk primer MOT. Gunakan SSR/contactor ber‑rating inrush; modul timer hanya bagian kontrol.
