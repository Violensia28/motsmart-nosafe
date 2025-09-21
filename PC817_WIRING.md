
# PC817 Driver for SSR (Recommended Wiring)

**Tujuan**: memutus ground loop & memberi isolasi saat mengendalikan input LED SSR dari ESP32.

## Opsi A — PC817 langsung (hanya jika input SSR <≈2–3 mA)
- **ESP32 GPIO26 (SSR_PIN)** → **R_LED (470Ω)** → **PC817 anoda**; **PC817 katoda** → **GND ESP32**.
- **Sisi transistor PC817**: **kolektor** → **SSR(+) via resistor**; **emitor** → **GND suplai SSR**.
- Gunakan suplai terpisah (mis. +5V) untuk input SSR, **jangan hubungkan GND-nya** ke ESP32 demi isolasi.
- Keterbatasan: CTR PC817 tipikal ~50–80% @ IF≈5mA, sehingga arus kolektor sering terbatas (≈2–4 mA), **mungkin tidak cukup** untuk banyak SSR yang butuh 5–10 mA.

## Opsi B — **Disarankan**: PC817 → NPN (2N2222) sebagai sink arus SSR
- **ESP32 GPIO26** → **R_LED 470Ω** → **PC817 anoda**; **PC817 katoda** → **GND ESP32**.
- **PC817 transistor**: **kolektor** → **+5V** melalui **R_base 10k** ke **basis 2N2222**; emitor PC817 → GND ESP32 (boleh disatukan di sisi logika saja). 
- **2N2222**: **kolektor** → **SSR(-)**, **SSR(+)** → **+5V**; **emitor** → **GND 5V**.
- Tambah **R_pull-down 100k** pada basis 2N2222 ke GND 5V agar OFF bersih.
- Kelebihan: mampu sink hingga >10 mA untuk LED input SSR dengan aman.

## Resistor LED PC817
- Target IF ≈ 4–6 mA. Vf LED PC817 ≈ 1.2V.
- **R = (3.3 - 1.2)/0.005 ≈ 420Ω** → pilih **470Ω** (aman untuk GPIO dan cukup terang).

> Catatan: Beberapa SSR memiliki input 3–32V dengan resistor internal; tetap perlakukan seperti LED dan batasi arus sesuai data SSR.

