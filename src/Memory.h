// Memory.h
#ifndef MEMORY_h
#define MEMORY_h

#include <LittleFS.h>
#include <Arduino.h>
#include <SPI.h>

typedef union
{
  float value;
  short int sValue;
  byte array[4];
}
specialFloatT;

class Memory {

  private:
    LittleFS_QSPIFlash flash;
    File flashFile;

    unsigned long start = 0;
    unsigned long end = 0;

    float log_freq = .1; // logging frequency in Hz (every 10 seconds)
    unsigned long interval = 1000 / log_freq; // logging interval in ms
    unsigned long last_log = 0;

    String data = "";
    String unlogged_data = "";

  public:
    void write_header();
    void write_data(specialFloatT* data);
    void erase_data();
    void dump_to_sd();
    void print();

    bool begin();

    Memory();
};

#endif