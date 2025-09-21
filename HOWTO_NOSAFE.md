
# HOWTO: Integrate NOSAFE quickly

1) Copy this pack to the root of your PlatformIO repo.
2) Append `platformio.additions.ini` to your `platformio.ini`.
3) In code:
   - `#include <SafetyPolicyNOSAFE.h>` & `#include <SensorsNOSAFE.h>`
   - Call `NOSAFE::begin(); SensorsNOSAFE::begin();` in `setup()`.
   - Call `SensorsNOSAFE::tick(); NOSAFE::tick(sdOK, SensorsNOSAFE::isOK());` in `loop()`.
4) Push to GitHub → CI builds `.bin` artifacts.

**Default pins (override via build_flags or Config.h):**
- DS18B20: GPIO4 with 4.7k pull-up to 3.3V
- PC817 input: GPIO27 (INPUT_PULLUP)
- ACS712 analog: GPIO34 (ESP32 ADC)

**Behavior:**
- Errors are **not blocking**. You will see warnings every **5 s**.
