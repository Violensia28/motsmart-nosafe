
#pragma once
#include <Arduino.h>

#ifndef MOTSMART_NOSAFE
#define MOTSMART_NOSAFE 1
#endif
#ifndef SAFETY_NOTIFY_ONLY
#define SAFETY_NOTIFY_ONLY 1
#endif

namespace NOSAFE {
  void begin();
  void tick(bool sd_ok, bool sensor_ok);
  inline bool safetyOK() { return true; }
}
