
#include "SafetyPolicyNOSAFE.h"

extern "C" void uiShowWarn(const char* msg) __attribute__((weak));
extern "C" void uiShowInfo(const char* msg) __attribute__((weak));

void uiShowWarn(const char* msg) { Serial.println(String("[WARN] ")+msg); }
void uiShowInfo(const char* msg) { Serial.println(String("[INFO] ")+msg); }

extern "C" void logRecord(const char* tag, const char* msg) __attribute__((weak));
void logRecord(const char* tag, const char* msg) { Serial.print("["); Serial.print(tag); Serial.print("] "); Serial.println(msg); }

namespace { uint32_t lastWarnMs=0; const uint32_t WARN_INTERVAL_MS=5000; }

namespace NOSAFE {
  void begin(){ if(!Serial){ Serial.begin(115200); delay(10);} uiShowInfo("NOSAFE policy active: notify-only"); }
  void tick(bool sd_ok, bool sensor_ok){
    const uint32_t now=millis();
    if((!sd_ok || !sensor_ok) && (now-lastWarnMs>=WARN_INTERVAL_MS)){
      lastWarnMs=now;
      if(!sd_ok){ logRecord("SAFETY","SD not detected / not OK (notify-only)"); uiShowWarn("SD ERROR: check card/cable"); }
      if(!sensor_ok){ logRecord("SAFETY","Sensor error (notify-only)"); uiShowWarn("SENSOR WARN: check probes"); }
    }
  }
}
