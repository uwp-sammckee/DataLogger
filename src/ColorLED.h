// ColorLED.h
#ifndef COLOR_LED_h
#define COLOR_LED_h

#include <Arduino.h>

class ColorLED {

  private:
    const int PIN_GREEN = 20;
    const int PIN_RED   = 21;
    const int PIN_BLUE  = 22;

    void setColor(int red, int green, int blue);

  public:
    void show_red();
    void show_green();
    void show_blue();
    void show_yellow();

    void begin();

    ColorLED();
};

#endif