
#!/data/data/com.termux/files/usr/bin/bash
set -e
if [ $# -lt 4 ]; then
  echo "Usage: $0 <PORT-or-URL> <bootloader.bin> <partitions.bin> <app.bin>"
  echo " PORT can be rfc2217://host:port or socket://host:port or /dev/ttyUSB0 (rooted)"
  exit 1
fi
PORT="$1"; BOOT="$2"; PART="$3"; APP="$4"
esptool --chip esp32 --port "$PORT" --baud 115200 write_flash   0x1000 "$BOOT" 0x8000 "$PART" 0x10000 "$APP"
