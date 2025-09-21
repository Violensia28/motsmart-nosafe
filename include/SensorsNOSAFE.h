
#pragma once
#include <Arduino.h>

namespace SensorsNOSAFE {
  struct Snapshot {
    bool ds18b20_ok;        // temperature within range & bus responsive
    float tempC;            // last temp reading
    bool pc817_ok;          // digital opto input health (active/high by default)
    bool acs_ok;            // ACS712 analog read sane
    float current_mA;       // estimated current (DC)
    uint32_t lastUpdateMs;  // timestamp of last refresh
  };

  void begin();
  void tick();            // call frequently; internally rate-limited
  bool isOK();            // aggregate health
  const Snapshot& get();  // latest snapshot
}
