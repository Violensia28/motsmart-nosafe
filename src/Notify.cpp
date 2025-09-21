
#include "Notify.h"
extern "C" void logRecord(const char* tag, const char* msg) __attribute__((weak));
static void log_fallback(const char* tag, const char* msg){ Serial.print("["); Serial.print(tag); Serial.print("] "); Serial.println(msg);} 
namespace Notify {
  void warn(const String& msg){ if(!Serial) Serial.begin(115200); if(logRecord) logRecord("WARN", msg.c_str()); else log_fallback("WARN", msg.c_str()); }
  void info(const String& msg){ if(!Serial) Serial.begin(115200); if(logRecord) logRecord("INFO", msg.c_str()); else log_fallback("INFO", msg.c_str()); }
}
