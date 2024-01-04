// ColorLED.cpp
#include "ColorLED.h"

void ColorLED::show_red(){
  setColor(1, 0, 0);  // red
}

void ColorLED::show_green(){
  setColor(0, 1, 0);  // green
}

void ColorLED::show_blue(){
  setColor(0, 0, 1);  // blue
}

void ColorLED::show_yellow(){
  setColor(1, 1, 0);  // yellow
}

void ColorLED::setColor(int red, int green, int blue)
{
  digitalWrite(PIN_RED, red);
  digitalWrite(PIN_GREEN, green);
  digitalWrite(PIN_BLUE, blue);  
}

void ColorLED::begin() {

  pinMode(PIN_RED,    OUTPUT);
  pinMode(PIN_BLUE,   OUTPUT);
  pinMode(PIN_GREEN,  OUTPUT); 
}

ColorLED::ColorLED(){};
