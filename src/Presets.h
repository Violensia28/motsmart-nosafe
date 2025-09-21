
#pragma once
#include <Arduino.h>
struct PulseSuggest { uint16_t pre_ms, delay_ms, main_ms, single_ms; };
inline PulseSuggest suggestByThickness(float tmm){
  if(tmm <= 0.10f) return {2,10,12,10};
  if(tmm <= 0.12f) return {2,10,14,12};
  if(tmm <= 0.15f) return {3,12,16,14};
  if(tmm <= 0.20f) return {3,12,20,18};
  return {4,12,24,24};
}
