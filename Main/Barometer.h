// Accelerometer.h
#ifndef BAROMETER_h
#define BAROMETER_h

#include <Arduino.h>
#include <Wire.h>
#include "Memory.h"

class Barometer {
  
  private:
    const int OUT_P_MSB = 0x01;
    const int OUT_T_MSB = 0x04;
    const int CTRL_REG1 = 0x26;

    const int BAROMETER = 0x60; // MPL3115A2 I2C address

    float alt, pres, temp;

    void update_sensor();

  public:
    void begin(int overSampleRate=7);
    void get_alt_pres_temp(specialFloatT* data);

    Barometer();
};

#endif
