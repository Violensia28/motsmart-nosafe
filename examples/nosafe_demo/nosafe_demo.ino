
#include <Arduino.h>
#include <SafetyPolicyNOSAFE.h>
#include <SensorsNOSAFE.h>

bool sdOK = true; // TODO: wire your SD status flag here if available

void setup(){
  Serial.begin(115200);
  delay(50);
  NOSAFE::begin();
  SensorsNOSAFE::begin();
}

void loop(){
  SensorsNOSAFE::tick();
  bool sensorsOK = SensorsNOSAFE::isOK();
  NOSAFE::tick(sdOK, sensorsOK);

  // demo print every second
  static uint32_t last=0; uint32_t now=millis();
  if(now-last>=1000){
    last=now;
    const auto& s = SensorsNOSAFE::get();
    Serial.printf("T=%.2fC, PC817=%s, I=%.1fmA, OK=%s
", s.tempC, s.pc817_ok?"HIGH":"LOW", s.current_mA, sensorsOK?"YES":"NO");
  }
}
