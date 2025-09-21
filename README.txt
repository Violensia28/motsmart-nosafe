
PlatformIO Project Root
=======================

File ini adalah `platformio.ini` minimal untuk env `manual-lite`.

Cara pakai:
1) Letakkan `platformio.ini` ini di **root** repo (bukan di subfolder).
2) Pastikan folder `src/` yang berisi `main.cpp`, `ConfigPins.h`, `Presets.h` sudah ada.
3) Push ke branch `manual-lite` → GitHub Actions (manual-lite-ci.yml) akan build.

Jika sebelumnya Anda sudah punya `platformio.ini`, cukup **tambahkan** blok `[env:manual-lite]` dari file ini ke bawah file Anda.
