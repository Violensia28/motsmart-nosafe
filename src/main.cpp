
#include <Arduino.h>

#ifndef TRIG_OUT_PIN
#define TRIG_OUT_PIN 26   // keluaran ke PC817 (sisi A +)
#endif
#ifndef SW_PIN
#define SW_PIN 27         // tombol manual (ke GND)
#endif
#ifndef BUZZER_PIN
#define BUZZER_PIN 19     // buzzer opsional
#endif

const uint16_t PULSE_MS    = 30;   // ketukan singkat 10–50 ms
const uint16_t DEBOUNCE_MS = 180;  // debouncing tombol

static void beep(uint16_t ms=60){
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(ms);
  digitalWrite(BUZZER_PIN, LOW);
}

// Kirim ketukan singkat lalu kembalikan pin ke Hi-Z (aman tanpa resistor)
static void pulseTimerTrigger(){
  pinMode(TRIG_OUT_PIN, OUTPUT);
  digitalWrite(TRIG_OUT_PIN, HIGH);
  delay(PULSE_MS);
  digitalWrite(TRIG_OUT_PIN, LOW);
  pinMode(TRIG_OUT_PIN, INPUT); // Hi-Z
}

void setup(){
  pinMode(SW_PIN, INPUT_PULLUP);    // tombol ke GND
  pinMode(TRIG_OUT_PIN, INPUT);     // default Hi-Z
  pinMode(BUZZER_PIN, OUTPUT);      // opsional
  digitalWrite(BUZZER_PIN, LOW);
}

void loop(){
  static bool last = true;             // idle HIGH (pull-up)
  bool now = digitalRead(SW_PIN);

  if (last && !now) {                  // tepi turun = ditekan
    beep(40);                          // feedback opsional
    pulseTimerTrigger();               // ketukan singkat ke modul timer
    delay(DEBOUNCE_MS);                // anti double-trigger
  }
  last = now;
}
