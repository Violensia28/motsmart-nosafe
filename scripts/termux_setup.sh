
#!/data/data/com.termux/files/usr/bin/bash
set -e
pkg update && pkg upgrade -y
pkg install -y python git unzip
python -m pip install --upgrade pip
pip install platformio esptool
echo "OK: PlatformIO & esptool installed"
