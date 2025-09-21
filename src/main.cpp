
#include <Arduino.h>
#include <U8g2lib.h>
#include "ConfigPins.h"
#include "Presets.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, OLED_SCL, OLED_SDA);

enum Mode { SINGLE=0, DOUBLE=1 };
const float THICK_PRESET_MM[] = {0.10f, 0.12f, 0.15f, 0.20f, 0.25f};
int thickIdx = 0; Mode modeSel = DOUBLE;
volatile int8_t encDelta=0; bool lastA=0, lastB=0; bool btnDown=false; bool armed=true;
uint16_t pre_ms=2, delay_ms=10, main_ms=12, single_ms=10;

void IRAM_ATTR isrEnc(){
  bool A=digitalRead(ROT_ENC_A); bool B=digitalRead(ROT_ENC_B);
  if(A!=lastA) encDelta += (A==B)? +1 : -1; lastA=A; lastB=B;
}

void applySuggest(){ auto s=suggestByThickness(THICK_PRESET_MM[thickIdx]); pre_ms=s.pre_ms; delay_ms=s.delay_ms; main_ms=s.main_ms; single_ms=s.single_ms; }

void pulseOn(uint16_t ms){ digitalWrite(SSR_PIN, HIGH); delay(ms); digitalWrite(SSR_PIN, LOW); }

void runWeld(){ armed=false; if(modeSel==SINGLE){ pulseOn(single_ms);} else { pulseOn(pre_ms); delay(delay_ms); pulseOn(main_ms);} delay(250); armed=true; }

void draw(){
  u8g2.clearBuffer(); u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.setCursor(0,12); u8g2.printf("M:%s  T:%.2f", modeSel==SINGLE?"Single":"Double", THICK_PRESET_MM[thickIdx]);
  u8g2.setCursor(0,28);
  if(modeSel==SINGLE) u8g2.printf("Single:%2d ms", single_ms);
  else u8g2.printf("P:%2d D:%2d M:%2d", pre_ms, delay_ms, main_ms);
  u8g2.setCursor(0,44); u8g2.print(armed?"READY (Trigger/Pen)":"WELDING...");
  u8g2.setCursor(0,60); u8g2.print("Rot:Edit  Btn:Switch Param");
  u8g2.sendBuffer();
}

enum EditField { EF_THICK=0, EF_MODE=1, EF_P1=2, EF_P2=3, EF_P3=4 };
EditField ef=EF_THICK; bool lastTrig=true, lastPen=true;

void setup(){
  pinMode(ROT_ENC_A, INPUT_PULLUP); pinMode(ROT_ENC_B, INPUT_PULLUP); pinMode(ROT_BTN, INPUT_PULLUP);
  pinMode(TRIGGER_BTN, INPUT_PULLUP); pinMode(PEN_SW_PIN, INPUT_PULLUP);
  pinMode(SSR_PIN, OUTPUT); digitalWrite(SSR_PIN, LOW);
  attachInterrupt(digitalPinToInterrupt(ROT_ENC_A), isrEnc, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROT_ENC_B), isrEnc, CHANGE);
  u8g2.begin(); u8g2.setPowerSave(0); applySuggest(); draw();
}

void loop(){
  int8_t d=encDelta; encDelta=0;
  if(d!=0){
    switch(ef){
      case EF_THICK: thickIdx = constrain(thickIdx + d, 0, (int)(sizeof(THICK_PRESET_MM)/sizeof(float))-1); applySuggest(); break;
      case EF_MODE:  modeSel  = (Mode)constrain((int)modeSel + d, 0, 1); break;
      case EF_P1:    if(modeSel==SINGLE) single_ms = constrain(single_ms + d, 1, 40); else pre_ms = constrain(pre_ms + d, 1, 10); break;
      case EF_P2:    if(modeSel==SINGLE){} else delay_ms = constrain(delay_ms + d, 4, 30); break;
      case EF_P3:    if(modeSel==SINGLE){} else main_ms  = constrain(main_ms + d, 4, 40); break;
    }
    draw();
  }
  bool rb = !digitalRead(ROT_BTN);
  if(rb && !btnDown){ btnDown=true; if(ef==EF_THICK) ef=EF_MODE; else if(ef==EF_MODE) ef=(modeSel==SINGLE?EF_P1:EF_P1); else if(ef==EF_P1) ef=(modeSel==SINGLE?EF_THICK:EF_P2); else if(ef==EF_P2) ef=EF_P3; else ef=EF_THICK; draw(); }
  else if(!rb) btnDown=false;
  bool trig=!digitalRead(TRIGGER_BTN); bool pen=!digitalRead(PEN_SW_PIN);
  if(armed && ((trig && !lastTrig) || (pen && !lastPen))){ draw(); runWeld(); draw(); }
  lastTrig=trig; lastPen=pen;
}
