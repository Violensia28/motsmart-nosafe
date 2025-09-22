
#include <Arduino.h>
#include <U8g2lib.h>

#ifndef OLED_SDA
#define OLED_SDA 21
#endif
#ifndef OLED_SCL
#define OLED_SCL 22
#endif
#ifndef OLED_ADDR
#define OLED_ADDR 0x3C
#endif
#ifndef ENC_A
#define ENC_A 32
#endif
#ifndef ENC_B
#define ENC_B 33
#endif
#ifndef ENC_BTN
#define ENC_BTN 25
#endif
#ifndef TRIG_BTN
#define TRIG_BTN 27
#endif
#ifndef SSR_PIN
#define SSR_PIN 26
#endif
#ifndef BUZZER_PIN
#define BUZZER_PIN 19
#endif
#ifndef PULSE_MIN_MS
#define PULSE_MIN_MS 4
#endif
#ifndef PULSE_MAX_MS
#define PULSE_MAX_MS 60
#endif
#ifndef PREHEAT_MAX_MS
#define PREHEAT_MAX_MS 10
#endif
#ifndef DELAY_MAX_MS
#define DELAY_MAX_MS 40
#endif

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, OLED_SCL, OLED_SDA);

// ==== UI/Encoder ====
volatile int8_t encDelta = 0; bool lastA=0, lastB=0;
static inline void IRAM_ATTR encISR(){
  bool A = digitalRead(ENC_A);
  bool B = digitalRead(ENC_B);
  if (A != lastA) encDelta += (A==B)? +1 : -1;
  lastA=A; lastB=B;
}

// ==== Mode & Params ====
enum Mode{ SINGLE=0, DOUBLE=1 };
Mode modeSel = DOUBLE;
uint16_t single_ms = 16;          // SINGLE
uint16_t pre_ms = 2, delay_ms=12, main_ms=18; // DOUBLE
bool armed = true;                 // ARM status

// Blink for selection highlight
const uint16_t BLINK_MS=320; bool blink=false; uint32_t tBlink=0;

// Selection fields
enum Field{ F_MODE=0, F_SINGLE, F_PRE, F_DLY, F_MAIN };
Field sel = F_MODE;

// Beep helpers
static inline void buzz_on(){ digitalWrite(BUZZER_PIN,HIGH);} 
static inline void buzz_off(){ digitalWrite(BUZZER_PIN,LOW);} 
static inline void beep(uint16_t ms=50){ buzz_on(); delay(ms); buzz_off(); }

// Fire SSR for ms
static void fireSSR(uint16_t ms){
  digitalWrite(SSR_PIN, HIGH);
  delay(ms);
  digitalWrite(SSR_PIN, LOW);
}

// Draw pretty header bar with icons
static void drawHeader(){
  u8g2.setDrawColor(1);
  u8g2.drawBox(0,0,128,14);
  u8g2.setDrawColor(0);
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.setCursor(2,11); u8g2.print("MANUAL·LITE v3");
  u8g2.setCursor(92,11); u8g2.print(armed?"ARM":"SAFE");
  u8g2.setDrawColor(1);
}

static void chip(uint8_t x,uint8_t y,uint8_t w,uint8_t h,bool on){
  if (on && blink) u8g2.drawBox(x,y,w,h); else u8g2.drawRFrame(x,y,w,h,3);
}

static void printIn(uint8_t x,uint8_t y,bool selected,const char* txt){
  if (selected && blink){ u8g2.setDrawColor(0); u8g2.setCursor(x,y); u8g2.print(txt); u8g2.setDrawColor(1);} else { u8g2.setCursor(x,y); u8g2.print(txt);} }

static void drawMain(){
  u8g2.clearBuffer();
  drawHeader();

  // MODE chip left
  u8g2.setFont(u8g2_font_10x20_tf);
  chip(4,18,60,22, sel==F_MODE);
  printIn(8,35, sel==F_MODE, (modeSel==SINGLE?"SINGLE":"DOUBLE"));

  // Right big time display
  u8g2.setFont(u8g2_font_fub20_tr);
  char buf[16];
  if (modeSel==SINGLE){ snprintf(buf,sizeof(buf),"%2ums", single_ms); }
  else { snprintf(buf,sizeof(buf),"%2u/%2u", pre_ms, main_ms); }
  printIn(72,36, false, buf);

  // Param chips bottom
  u8g2.setFont(u8g2_font_6x12_tf);
  if (modeSel==SINGLE){
    chip(4,42,120,14, sel==F_SINGLE);
    if (sel==F_SINGLE && blink){ u8g2.setDrawColor(0);} 
    u8g2.setCursor(8,52); u8g2.print("Single: "); u8g2.print(single_ms); u8g2.print(" ms");
    if (sel==F_SINGLE && blink){ u8g2.setDrawColor(1);} 
  } else {
    // labels
    u8g2.setCursor(6,54); u8g2.print("Pre");
    u8g2.setCursor(52,54); u8g2.print("Dly");
    u8g2.setCursor(96,54); u8g2.print("Main");
    // chips
    chip(4,42,34,12, sel==F_PRE);  printIn(10,51, sel==F_PRE,  String(pre_ms).c_str());
    chip(48,42,34,12, sel==F_DLY); printIn(54,51, sel==F_DLY,  String(delay_ms).c_str());
    chip(92,42,34,12, sel==F_MAIN);printIn(98,51, sel==F_MAIN, String(main_ms).c_str());
  }

  // footer hints
  u8g2.setCursor(0,64); u8g2.print("ENC=Edit  BTN=Next  TRIG");

  u8g2.sendBuffer();
}

static void runWeld(){
  if (!armed) return;
  armed=false;
  // Splash
  u8g2.clearBuffer(); drawHeader();
  u8g2.setFont(u8g2_font_10x20_tf); u8g2.setCursor(6,40); u8g2.print("WELDING");
  u8g2.sendBuffer();

  if (modeSel==SINGLE){ fireSSR(single_ms); }
  else { fireSSR(pre_ms); delay(delay_ms); fireSSR(main_ms); }

  beep(80); delay(250); armed=true; drawMain();
}

void setup(){
  pinMode(ENC_A, INPUT_PULLUP); pinMode(ENC_B, INPUT_PULLUP); pinMode(ENC_BTN, INPUT_PULLUP);
  pinMode(TRIG_BTN, INPUT_PULLUP);
  pinMode(SSR_PIN, OUTPUT); digitalWrite(SSR_PIN, LOW);
  pinMode(BUZZER_PIN, OUTPUT); digitalWrite(BUZZER_PIN, LOW);
  attachInterrupt(digitalPinToInterrupt(ENC_A), encISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), encISR, CHANGE);

  u8g2.setI2CAddress(OLED_ADDR<<1); u8g2.begin(); u8g2.setPowerSave(0);
  drawMain();
}

void loop(){
  // Encoder delta
  int8_t d = encDelta; encDelta=0;
  if (d){
    switch(sel){
      case F_MODE:   modeSel = (Mode)constrain((int)modeSel + d, 0, 1); break;
      case F_SINGLE: single_ms = constrain((int)single_ms + d, PULSE_MIN_MS, PULSE_MAX_MS); break;
      case F_PRE:    pre_ms    = constrain((int)pre_ms + d, 1, PREHEAT_MAX_MS); break;
      case F_DLY:    delay_ms  = constrain((int)delay_ms + d, 4, DELAY_MAX_MS); break;
      case F_MAIN:   main_ms   = constrain((int)main_ms + d, PULSE_MIN_MS, PULSE_MAX_MS); break;
    }
    drawMain();
  }

  // Encoder button → next field
  static bool ebPrev = false; bool eb = !digitalRead(ENC_BTN);
  if (eb && !ebPrev){
    if (sel==F_MODE) sel = (modeSel==SINGLE? F_SINGLE : F_PRE);
    else if (sel==F_SINGLE) sel=F_MODE;
    else if (sel==F_PRE) sel=F_DLY;
    else if (sel==F_DLY) sel=F_MAIN;
    else sel=F_MODE;
    beep(30); drawMain();
  }
  ebPrev = eb;

  // Trigger button (active LOW)
  static bool tbPrev=true; bool tb = digitalRead(TRIG_BTN)==LOW;
  if (tb && !tbPrev){ beep(40); runWeld(); }
  tbPrev=tb;

  // Blink timer
  if (millis()-tBlink>=BLINK_MS){ tBlink=millis(); blink=!blink; if (armed) drawMain(); }
}
