
# Termux: Build & (Options to) Flash from Android

> **Heads-up:** Android **does not expose** /dev/ttyUSB* to user apps by default. Termux provides `termux-usb` which
> hands you a **file descriptor via Java API**, not a TTY device. Tools like `esptool.py` expect a serial TTY. See Termux wiki.\[1]> For flashing, pick **one** of the methods below.

## A) Build firmware locally in Termux (recommended)
1. Install Termux (F-Droid) and Termux:API.
2. In Termux:
   ```bash
   pkg update && pkg upgrade -y
   pkg install -y python git unzip
   python -m pip install --upgrade pip
   pip install platformio esptool
   ```
3. Clone your repo & build:
   ```bash
   git clone <YOUR_REPO_URL> mot-smart
   cd mot-smart
   pio run -e nosafe-full   # or nosafe-minimal
   ```
   Artifacts appear under `.pio/build/<env>/`.

## B) Flash via **remote serial** using RFC2217
If you can bridge your phone's USB serial to a **network socket** that supports **RFC2217**, `esptool` can flash over network.\[2]\[3]
- On **the bridge** (another device **or** an Android app that exposes RFC2217): it must map the ESP32 USB serial to `rfc2217://<host>:<port>`.
- In Termux (client):
  ```bash
  esptool --chip esp32 --port rfc2217://<HOST>:<PORT>?ign_set_control --baud 115200 flash_id
  esptool --chip esp32 --port rfc2217://<HOST>:<PORT>?ign_set_control --baud 115200     write_flash 0x1000 bootloader.bin 0x8000 partitions.bin 0x10000 app.bin
  ```
  *Note:* automatic DTR/RTS reset may be limited; `?ign_set_control` helps with latency.\[2]

## C) Flash via **raw TCP socket** (e.g., apps like **TCPUART**)
Some Android serial bridge apps offer a **plain TCP server** (not RFC2217). Esptool supports `socket://` mode with reduced features: **no baud switching nor auto reset**; you must set baud **in the app** and manually hold **BOOT/EN** on the ESP32.\[2]
```bash
esptool --chip esp32 -p socket://127.0.0.1:6667 flash_id
# then write_flash as above (manually reset device into bootloader)
```

## D) Flash with **rooted** Android (+ kernel driver)
If your phone is rooted and kernel has usb-serial modules (`usbserial`, `ch341`, `ftdi_sio`, etc.), you can load them and get `/dev/ttyUSB0`, then use esptool normally.\[4]

---

### Offsets & files to flash
Typical ESP32 images (Arduino/PlatformIO) use: bootloader @ **0x1000**, partitions @ **0x8000**, app @ **0x10000**.\[5]\[6]\[7]
```bash
esptool --chip esp32 --baud 921600 --port <PORT> write_flash   0x1000 bootloader.bin   0x8000 partitions.bin   0x10000 firmware.bin
```

### Links
[1] Termux USB limitations & example (`termux-usb`)\[TERMUX]
[2] Esptool Remote Serial Ports (RFC2217 & socket://)\[ESP_REM]
[3] Esptool Advanced Topics index\[ESP_ADV]
[4] XDA: Root + kernel modules to get /dev/ttyUSB*\[XDA]
[5] Esptool basic commands & `write_flash`\[ESP_BAS]
[6] ESP-IDF bootloader location (0x1000)\[ESP_BOOT]
[7] ESP-IDF partition table & offsets (0x8000, 0x10000)\[ESP_PART]
