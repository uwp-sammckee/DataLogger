// Memory.h
#ifndef MEMORY_h
#define MEMORY_h

#include <Adafruit_SPIFlash.h>
#include <Arduino.h>
#include <SPI.h>

#include "Data.hpp"

class Memory {
  private:
    const int FLASH_SCK = 0;
    const int FLASH_CS  = 0;
    const int FLASH_IO0 = 0;
    const int FLASH_IO1 = 0;
    const int FLASH_IO2 = 0;
    const int FLASH_IO3 = 0;

    Adafruit_FlashTransport_SPI flashTransport;
    Adafruit_SPIFlash flash;
    const SPIFlash_Device_t flashIC = W25Q128JV_SQ;

    unsigned long start = 0;
    unsigned long end = 0;

    float log_freq = .1; // logging frequency in Hz (every 10 seconds)
    unsigned long interval = 10000; // override to every 10 sec // = 1000 / log_freq; // logging interval in ms
    unsigned long lastLog = 0;

    String data = "";
    String unloggedData = "";

  public:
    const String header = "Time,AccX,AccY,AccZ,GyrX,GyrY,GyrZ,MagX,MagY,MagZ,Heading,AngleX,AngleY,AngleZ,VelX,VelY,VelZ,Temp,Press,Alti,State,GPS_LAT,GPS_LNG,GPS_SAT,GPS_ALT,GPS_SPEED,GPS_HDOP";
    
    void write_header() {};
    void write_data(Data *data);
    void erase_data();
    void dump_to_sd();
    void print();

    bool begin();

    Memory();
};

#endif