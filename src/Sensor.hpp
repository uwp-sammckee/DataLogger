// Sensor.hpp
#ifndef SENSOR_h
#define SENSOR_h

#include <Arduino.h>
#include <Wire.h>
#include "Memory.h"

class Sensor {
  
  private:
    TwoWire *wire;
    int address;

    virtual void update_sensor();

  protected:
    void read(byte registerAddress, byte* data, int bytes=1) {
        wire->beginTransmission(address);
        
        wire->write(registerAddress);
        wire->endTransmission(false);

        wire->requestFrom(address, bytes);
        for (int i=0; i<bytes; i++)
            data[i] = wire->read();

        wire->endTransmission(true);
    }

    void write(byte registerAddress, byte data) {
        wire->beginTransmission(address);
        
        wire->write(registerAddress);
        wire->write(data);

        wire->endTransmission(true);
    }

  public:
    // virtual void begin();
    virtual void get_data(specialFloatT* data);

    Sensor(int address, TwoWire *wire=&Wire) {
        this->address = address;
        this->wire = wire;
    }
};

#endif
