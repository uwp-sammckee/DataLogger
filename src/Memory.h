// Memory.h
#ifndef MEMORY_h
#define MEMORY_h

#include <LittleFS.h>
#include <Arduino.h>
#include <SPI.h>

#include <RingBuf.h>
#include <SdFat.h>

#define SD_CS_PIN SDCARD_SS_PIN

#define SD_CONFIG SdioConfig(FIFO_SDIO)

typedef union
{
  float value;
  byte array[4];
}
specialFloatT;

class Memory {
  private:
    // Flash
    LittleFS_QSPIFlash flash;
    File flashFile;

    // SD
    SdExFat sd;
    ExFile sdFile;


    unsigned long start = 0;
    unsigned long end = 0;

    float log_freq = .1; // logging frequency in Hz (every 10 seconds)
    unsigned long interval = 30000; // override to every 30 sec // = 1000 / log_freq; // logging interval in ms
    unsigned long lastLog = 0;

    String data = "";
    String unloggedData = "";

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