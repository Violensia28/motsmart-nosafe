
# Timer Trigger Mode — Repo‑Ready

## Cara pakai (3 langkah)
1) Upload **isi folder ini** ke repo GitHub (branch `manual-lite`) → centang *Delete existing files* bila perlu.
2) Actions berjalan otomatis → ambil artifacts:
   - `firmware-manual-lite/firmware.bin` → flash @ `0x10000`
   - `flash-bundle-manual-lite` (ZIP) → paket lengkap untuk ESP32 Flash Tool (Android)
3) Sambungkan hardware sesuai **Wiring** di bawah.

## Wiring (tanpa menyebut pin IC, pakai terminal modul)
- **ESP32 TRIG_OUT (GPIO26)** → **PC817 sisi A (+)**
- **ESP32 G** → **PC817 sisi A (−)**
- **IN1 (modul timer)** → **PC817 sisi B (OUT)**
- **G (modul timer)** → **PC817 sisi B (G)**
- **V2 (modul timer)** → **+ supply modul (5V/12V sesuai modul)**
- **G (modul timer)** → **0V modul**

> Dengan ini, saat ESP32 memberi ketukan, **IN1** tertarik ke **G** sebentar (low‑level trigger). Pastikan modul di mode **Monostable** dan `t_on` disetel sesuai kebutuhan.

## Peringatan
- **Jangan** gunakan relay kecil dari modul timer untuk **primer MOT** (arus inrush tinggi). Gunakan output relay modul untuk **mengendalikan SSR/contactor** yang sesuai rating.
- Tanpa resistor seri di PC817: ketukan ESP32 **sangat singkat** (30 ms) dan pin dikembalikan **Hi‑Z** untuk menghindari arus berlebih.

## Uji cepat
1) Set modul timer: **Monostable**, trigger **low‑level**, durasi **t_on**.
2) Tekan tombol (GPIO27 ke GND) → modul timer ON selama **t_on** → OFF sendiri.
3) Uji beban ringan (lampu) sebelum ke aktuator daya sebenarnya.
