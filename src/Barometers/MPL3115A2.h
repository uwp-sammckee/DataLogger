// Barometers/MPL3115A2.h
#ifndef MPL3115A2_h
#define MPL3115A2_h

#include <Arduino.h>
#include <Wire.h>

#include "Sensor.hpp"
#include "Memory.h"

#define MPL3115A2_ADDR 0x60

class MPL3115A2 : Sensor {
  
  private:
    const int OUT_P_MSB = 0x01;
    const int OUT_T_MSB = 0x04;
    const int CTRL_REG1 = 0x26;


    float alt, pres, temp;

    void update_sensor();

  public:
    void begin(int overSampleRate=7);
    void get_data(specialFloatT* data);

    MPL3115A2(int address=MPL3115A2_ADDR, TwoWire *wire=&Wire);
};

#endif
