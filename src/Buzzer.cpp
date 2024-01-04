// Buzzer.cpp
#include "Buzzer.h"

void Buzzer::end_sound(){
  tone(PIN_BUZZER, 400);
  delay(1000);
  noTone(PIN_BUZZER);
}

void Buzzer::start_sound() {
  tone(PIN_BUZZER, 1000);
  delay(200);
  noTone(PIN_BUZZER);
}

void Buzzer::start_up_sound() {
  for (int i=100; i<=1000; i+=100){
    tone(PIN_BUZZER, i);
    delay(100);
  }
  noTone(PIN_BUZZER);
}

int Buzzer::get_buzzer_pin(){
  return PIN_BUZZER;
}

void Buzzer::begin() {
  pinMode(PIN_BUZZER, OUTPUT);
}

Buzzer::Buzzer(){
  // pinMode(PIN_BUZZER, OUTPUT);
}