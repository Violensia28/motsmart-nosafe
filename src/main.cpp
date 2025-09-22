
#include <Arduino.h>
#include <U8g2lib.h>

#ifndef OLED_SDA
#define OLED_SDA 21
#endif
#ifndef OLED_SCL
#define OLED_SCL 22
#endif
#ifndef ROT_ENC_A
#define ROT_ENC_A 32
#endif
#ifndef ROT_ENC_B
#define ROT_ENC_B 33
#endif
#ifndef ROT_BTN
#define ROT_BTN 25
#endif
#ifndef TRIGGER_BTN
#define TRIGGER_BTN 27
#endif
#ifndef SSR_PIN
#define SSR_PIN 26
#endif
#ifndef PEN_SW_PIN
#define PEN_SW_PIN 14
#endif
#ifndef BUZZER_PIN
#define BUZZER_PIN 19
#endif

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, OLED_SCL, OLED_SDA);

enum Mode { SINGLE=0, DOUBLE=1 };
const float THICK_PRESET_MM[] = {0.10f, 0.12f, 0.15f, 0.20f, 0.25f};
int thickIdx = 1; // default 0.12 mm
Mode modeSel = DOUBLE;

volatile int8_t encDelta = 0; bool lastA=0, lastB=0; bool btnDown=false;

uint16_t pre_ms=2, delay_ms=10, main_ms=14, single_ms=12;
bool armed=true; bool lastTrig=true, lastPen=true;

inline void buzzer_on(){ digitalWrite(BUZZER_PIN, HIGH);} 
inline void buzzer_off(){ digitalWrite(BUZZER_PIN, LOW);} 
void beep(uint16_t ms=60){ buzzer_on(); delay(ms); buzzer_off(); }

void IRAM_ATTR isrEnc(){
  bool A = digitalRead(ROT_ENC_A);
  bool B = digitalRead(ROT_ENC_B);
  if (A != lastA) encDelta += (A==B) ? +1 : -1; lastA=A; lastB=B;
}

struct PulseSuggest { uint16_t pre_ms, delay_ms, main_ms, single_ms; };
PulseSuggest suggestByThickness(float tmm){
  if (tmm <= 0.10f) return {2,10,12,10};
  if (tmm <= 0.12f) return {2,10,14,12};
  if (tmm <= 0.15f) return {3,12,16,14};
  if (tmm <= 0.20f) return {3,12,20,18};
  return {4,12,24,24};
}

void applySuggest(){ auto s = suggestByThickness(THICK_PRESET_MM[thickIdx]); pre_ms=s.pre_ms; delay_ms=s.delay_ms; main_ms=s.main_ms; single_ms=s.single_ms; }

void pulseOn(uint16_t ms){ digitalWrite(SSR_PIN, HIGH); delay(ms); digitalWrite(SSR_PIN, LOW); }

void runWeld(){
  armed=false; 
  // Splash welding screen
  u8g2.clearBuffer();
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.drawBox(0,0,128,14); // header invert
  u8g2.setDrawColor(0); u8g2.setCursor(2,11); u8g2.print("WELDING"); u8g2.setDrawColor(1);
  u8g2.setCursor(90,11); u8g2.print(modeSel==SINGLE?"SINGLE":"DOUBLE");
  u8g2.setFont(u8g2_font_10x20_tf);
  if (modeSel==SINGLE) {
    u8g2.setCursor(8,40); u8g2.print("Single");
    u8g2.setCursor(90,40); u8g2.print(single_ms);
  } else {
    u8g2.setCursor(4,34);  u8g2.print("P:"); u8g2.setCursor(30,34); u8g2.print(pre_ms);
    u8g2.setCursor(64,34); u8g2.print("D:"); u8g2.setCursor(90,34); u8g2.print(delay_ms);
    u8g2.setCursor(4,60);  u8g2.print("M:"); u8g2.setCursor(30,60); u8g2.print(main_ms);
  }
  u8g2.sendBuffer();

  if (modeSel==SINGLE) { pulseOn(single_ms); }
  else { pulseOn(pre_ms); delay(delay_ms); pulseOn(main_ms); }

  beep(80); delay(250); armed=true; 
}

void drawHeader(const char* status){
  u8g2.setDrawColor(1);
  u8g2.drawBox(0,0,128,14);
  u8g2.setDrawColor(0);
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.setCursor(2,11); u8g2.print("MANUAL·LITE");
  u8g2.setCursor(88,11); u8g2.print(status);
  u8g2.setDrawColor(1);
}

void drawFooter(){
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.drawHLine(0,54,128);
  u8g2.setCursor(0,64);  u8g2.print("ENC:Edit");
  u8g2.setCursor(50,64); u8g2.print("BTN:Next");
  u8g2.setCursor(98,64); u8g2.print("TRIG");
}

void drawMain(){
  u8g2.clearBuffer();
  drawHeader(armed?"READY":"BUSY");

  // Mode chip + Thickness besar
  u8g2.setFont(u8g2_font_10x20_tf);
  // Mode chip
  u8g2.drawRFrame(4,18,60,22,4);
  u8g2.setCursor(8,35); u8g2.print(modeSel==SINGLE?"SINGLE":"DOUBLE");

  // Ketebalan besar kanan
  char buf[16];
  dtostrf(THICK_PRESET_MM[thickIdx], 0, 2, buf);
  u8g2.setCursor(74,35); u8g2.print(buf); u8g2.print("mm");

  // Kartu parameter
  u8g2.setFont(u8g2_font_6x12_tf);
  if (modeSel==SINGLE) {
    u8g2.drawRFrame(4,38,120,14,3);
    u8g2.setCursor(8,49); u8g2.print("Single:");
    u8g2.setCursor(56,49); u8g2.print(single_ms); u8g2.print(" ms");
  } else {
    u8g2.drawRFrame(4,40,120,12,3);
    u8g2.setCursor(8,49);  u8g2.print("P:"); u8g2.setCursor(24,49); u8g2.print(pre_ms);
    u8g2.setCursor(54,49); u8g2.print("D:"); u8g2.setCursor(70,49); u8g2.print(delay_ms);
    u8g2.setCursor(96,49); u8g2.print("M:"); u8g2.setCursor(112,49); u8g2.print(main_ms);
  }

  drawFooter();
  u8g2.sendBuffer();
}

enum EditField { EF_THICK=0, EF_MODE=1, EF_P1=2, EF_P2=3, EF_P3=4 };
EditField ef = EF_THICK;

void setup(){
  pinMode(ROT_ENC_A, INPUT_PULLUP); pinMode(ROT_ENC_B, INPUT_PULLUP); pinMode(ROT_BTN, INPUT_PULLUP);
  pinMode(TRIGGER_BTN, INPUT_PULLUP); pinMode(PEN_SW_PIN, INPUT_PULLUP);
  pinMode(SSR_PIN, OUTPUT); digitalWrite(SSR_PIN, LOW);
  pinMode(BUZZER_PIN, OUTPUT); digitalWrite(BUZZER_PIN, LOW);
  attachInterrupt(digitalPinToInterrupt(ROT_ENC_A), isrEnc, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROT_ENC_B), isrEnc, CHANGE);
  u8g2.begin(); u8g2.setPowerSave(0);
  applySuggest();
  drawMain();
}

void loop(){
  // Encoder delta
  int8_t d = encDelta; encDelta = 0;
  if (d){
    switch(ef){
      case EF_THICK: thickIdx = constrain(thickIdx+d, 0, (int)(sizeof(THICK_PRESET_MM)/sizeof(float))-1); applySuggest(); break;
      case EF_MODE:  modeSel  = (Mode)constrain((int)modeSel + d, 0, 1); break;
      case EF_P1:    if (modeSel==SINGLE) single_ms = constrain(single_ms + d, 1, 40); else pre_ms = constrain(pre_ms + d, 1, 10); break;
      case EF_P2:    if (modeSel==SINGLE) {} else delay_ms = constrain(delay_ms + d, 4, 30); break;
      case EF_P3:    if (modeSel==SINGLE) {} else main_ms  = constrain(main_ms + d, 4, 40); break;
    }
    drawMain();
  }

  // Button encoder: pindah field
  bool rb = !digitalRead(ROT_BTN);
  static bool rbPrev=false;
  if (rb && !rbPrev){
    if (ef==EF_THICK) ef=EF_MODE;
    else if (ef==EF_MODE) ef = (modeSel==SINGLE? EF_P1 : EF_P1);
    else if (ef==EF_P1) ef = (modeSel==SINGLE? EF_THICK : EF_P2);
    else if (ef==EF_P2) ef = EF_P3;
    else ef = EF_THICK;
    drawMain();
  }
  rbPrev = rb;

  // Trigger & pen
  bool trig = !digitalRead(TRIGGER_BTN);
  bool pen  = !digitalRead(PEN_SW_PIN);

  // Edge press → bip
  if ((trig && !lastTrig) || (pen && !lastPen)) beep(60);

  // Eksekusi
  if (armed && ((trig && !lastTrig) || (pen && !lastPen))) runWeld();

  lastTrig=trig; lastPen=pen;
}
