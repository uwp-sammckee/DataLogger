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
    void read(int registerAddress, byte* data, int bytes=1) {
      wire->beginTransmission(address);
      
      wire->write(registerAddress);
      wire->endTransmission(false);

      wire->requestFrom(address, bytes);
      for (int i=0; i<bytes; i++)
        data[i] = wire->read();

      wire->endTransmission(true);
    }

    void write(int registerAddress, byte data) {
      wire->beginTransmission(address);
      
      wire->write(registerAddress);
      wire->write(data);

      wire->endTransmission(true);
    }

    float read_float(int registerAddress, float scale) {
      byte bytes[2];
      read(registerAddress, bytes, 2);

      int16_t raw_float = bytes[0] | (bytes[1] << 8);

      return static_cast<float>(raw_float) / 16.0;
    }

    byte read_byte(int registerAddress) {
      byte data;
      read(registerAddress, &data);
      return data;
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
