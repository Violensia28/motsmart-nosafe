
#include "SensorsNOSAFE.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ACS712.h>

#ifndef PIO_DEF_PIN_ONEWIRE
#define PIO_DEF_PIN_ONEWIRE 4
#endif
#ifndef PIO_DEF_PIN_PC817
#define PIO_DEF_PIN_PC817 27
#endif
#ifndef PIO_DEF_PIN_ACS712
#define PIO_DEF_PIN_ACS712 34
#endif
#ifndef PIO_DEF_ACS_MV_PER_A
#define PIO_DEF_ACS_MV_PER_A 100
#endif
#ifndef PIO_DEF_ACS_VREF
#define PIO_DEF_ACS_VREF 3.3
#endif
#ifndef PIO_DEF_ACS_MAXADC
#define PIO_DEF_ACS_MAXADC 4095
#endif
#ifndef PIO_DEF_TEMP_MIN_C
#define PIO_DEF_TEMP_MIN_C 0
#endif
#ifndef PIO_DEF_TEMP_MAX_C
#define PIO_DEF_TEMP_MAX_C 85
#endif

namespace {
  OneWire oneWire(PIO_DEF_PIN_ONEWIRE);
  DallasTemperature dallas(&oneWire);
  ACS712 acs(PIO_DEF_PIN_ACS712, PIO_DEF_ACS_VREF, PIO_DEF_ACS_MAXADC, PIO_DEF_ACS_MV_PER_A);

  SensorsNOSAFE::Snapshot snap{};
  uint32_t lastTick=0;            // sensor sampling throttle
  const uint32_t TICK_INTERVAL_MS=200;   // lightweight tick
  uint32_t lastTempReq=0;
  const uint32_t TEMP_INTERVAL_MS=1000;  // request new temp each second (async)
}

namespace SensorsNOSAFE {
  void begin(){
    pinMode(PIO_DEF_PIN_PC817, INPUT_PULLUP); // assumes active-low opto to GND; invert below if needed
    dallas.begin();
    dallas.setWaitForConversion(false);       // async conversion to avoid blocking
    dallas.requestTemperatures();
    acs.autoMidPointDC(50);                   // quick midpoint calibration assuming ~0A
    snap.lastUpdateMs = millis();
  }

  void tick(){
    const uint32_t now = millis();
    if(now - lastTick < TICK_INTERVAL_MS) return;
    lastTick = now;

    // DS18B20: read last conversion, then request next
    float t = dallas.getTempCByIndex(0);
    bool busOK = (t > -127.0f && t < 125.0f); // -127 indicates disconnected
    if(busOK) snap.tempC = t;
    if(now - lastTempReq >= TEMP_INTERVAL_MS){ dallas.requestTemperatures(); lastTempReq = now; }

    // Health: within sane limits
    snap.ds18b20_ok = busOK && (snap.tempC >= PIO_DEF_TEMP_MIN_C) && (snap.tempC <= PIO_DEF_TEMP_MAX_C);

    // PC817 digital (active-low typical). Consider OK if pin reads HIGH (no fault), adjust if wiring differs.
    int v = digitalRead(PIO_DEF_PIN_PC817);
    snap.pc817_ok = (v == HIGH);

    // ACS712 DC sampling (fast); sanity: value finite and not NaN
    float mA = acs.mA_DC(10);
    if(!isnan(mA) && isfinite(mA)){
      snap.current_mA = mA;
      snap.acs_ok = true; // more detailed checks can be added (range windows)
    } else {
      snap.acs_ok = false;
    }

    snap.lastUpdateMs = now;
  }

  bool isOK(){
    return snap.ds18b20_ok && snap.pc817_ok && snap.acs_ok;
  }

  const Snapshot& get(){ return snap; }
}
