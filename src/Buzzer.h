// Buzzer.h
#ifndef BUZZER_h
#define BUZZER_h

#include <Arduino.h>

class Buzzer {

  private:
    const int PIN_BUZZER = 17;

  public:
    void end_sound();
    void start_sound();
    void start_up_sound();

    int get_buzzer_pin();

    Buzzer();
};
#endif