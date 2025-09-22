
#include <Arduino.h>
#include <U8g2lib.h>
#ifndef TRIG_OUT_PIN
#define TRIG_OUT_PIN 26
#endif
#ifndef SW_PIN
#define SW_PIN 27
#endif
#ifndef BUZZER_PIN
#define BUZZER_PIN 19
#endif
#ifndef OLED_SDA
#define OLED_SDA 21
#endif
#ifndef OLED_SCL
#define OLED_SCL 22
#endif
#ifndef OLED_ADDR
#define OLED_ADDR 0x3C
#endif
#ifndef PULSE_MS
#define PULSE_MS 120
#endif
#ifndef DEBOUNCE_MS
#define DEBOUNCE_MS 180
#endif
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, OLED_SCL, OLED_SDA);
static void beep(uint16_t ms=60){ pinMode(BUZZER_PIN, OUTPUT); digitalWrite(BUZZER_PIN, HIGH); delay(ms); digitalWrite(BUZZER_PIN, LOW); }
static void drawStatus(const char* top, const char* mid, const char* bot){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.drawBox(0,0,128,14); u8g2.setDrawColor(0); u8g2.setCursor(2,11); u8g2.print(top); u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_10x20_tf); u8g2.setCursor(4,40); u8g2.print(mid);
  u8g2.setFont(u8g2_font_6x12_tf);  u8g2.setCursor(4,62); u8g2.print(bot);
  u8g2.sendBuffer();
}
static void pulseTimerTrigger(){
  pinMode(TRIG_OUT_PIN, OUTPUT);
  digitalWrite(TRIG_OUT_PIN, HIGH);
  delay(PULSE_MS);
  digitalWrite(TRIG_OUT_PIN, LOW);
  pinMode(TRIG_OUT_PIN, INPUT);
}
void setup(){
  pinMode(SW_PIN, INPUT_PULLUP);
  pinMode(TRIG_OUT_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT); digitalWrite(BUZZER_PIN, LOW);
  u8g2.setI2CAddress(OLED_ADDR<<1);
  u8g2.begin(); u8g2.setPowerSave(0);
  drawStatus("TIMER MODE","READY","BTN->TRIG  ENC N/A");
}
void loop(){
  static bool last = true;
  bool now = digitalRead(SW_PIN);
  if (last && !now){
    beep(40);
    drawStatus("TIMER MODE","TRIGGER","PULSE 120 ms");
    pulseTimerTrigger();
    delay(DEBOUNCE_MS);
    drawStatus("TIMER MODE","READY","BTN->TRIG  ENC N/A");
  }
  last = now;
}
