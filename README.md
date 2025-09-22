
# Micro Switch → ESP32 → PC817 → Timer (OLED) — Repo‑Ready

## Cara pakai
1) Upload isi folder ini ke branch `manual-lite` (centang *Delete existing files*). 
2) Actions build otomatis → ambil `.bin` & Flash ZIP dari artifacts.
3) Lihat diagram di `diagrams/wiring_microswitch_esp32_pc817_timer_oled.png`.

## Wiring ringkas (terminal, tanpa sebutan kaki):
- **Micro switch → ESP32**: Lug 1 → GPIO27, Lug 2 → GND
- **ESP32 → PC817 (input)**: TRIG_OUT (GPIO26) → **V1**, GND → **G**
- **PC817 (output) → Timer**: **IN1** → **Trigger**, **G (output)** → **GND Trigger**
- **Power Timer**: USB 5V (atau 6–30V ke terminal sesuai modul)
- **OLED**: VCC→3V3, GND→GND, SDA→GPIO21, SCL→GPIO22 (addr 0x3C)
- **Relay Timer (NO, COM, NC)**: gunakan **NO–COM** untuk kontrol SSR/contactor (disarankan)

## Catatan keselamatan
- Relay kecil pada modul timer **jangan** untuk primer MOT. Pakai SSR/contactor ber‑rating inrush.
- Uji dulu dengan **beban lampu** sebelum ke aktuator daya sesungguhnya.

