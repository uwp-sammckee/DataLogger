// GPS.h
#ifndef GPS_h
#define GPS_h

#include <Arduino.h>
#include <TinyGPSPlus.h>

#include "Memory.h"
#include "Sensor.hpp"

class GPS : public Sensor {

  private:
    TinyGPSPlus gps;
    HardwareSerial* serial; // The serial port the GPS is connected to
    int baud;               // The baud rate of the GPS  

    int freq = 10; // Update frequency in Hz
    unsigned long interval = freq / 1000.0;
    unsigned long last = 0.0;

  public:
    bool begin() override;
    void update_sensor() override;
    void get_data(specialFloatT* data) override;

    GPS(HardwareSerial* serial, int baud=9600);
};

#endif