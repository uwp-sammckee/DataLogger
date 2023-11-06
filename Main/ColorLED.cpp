// ColorLED.cpp
#include "ColorLED.h"

void ColorLED::show_red(){
  setColor(255, 0, 0);  // red
}

void ColorLED::show_green(){
  setColor(0, 255, 0);  // green
}

void ColorLED::show_blue(){
  setColor(0, 0, 255);  // blue
}

void ColorLED::show_yellow(){
  setColor(255, 255, 0);  // yellow
}

int ColorLED::get_red_pin(){
  return ColorLED::PIN_RED;
}

int ColorLED::get_green_pin(){
  return PIN_GREEN;
}

int ColorLED::get_blue_pin(){
  return PIN_BLUE;
}

void ColorLED::setColor(int red, int green, int blue)
{
  analogWrite(PIN_RED, red);
  analogWrite(PIN_GREEN, green);
  analogWrite(PIN_BLUE, blue);  
}

ColorLED::ColorLED(){};
