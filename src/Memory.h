// Memory.h
#ifndef MEMORY_h
#define MEMORY_h

#include <Arduino.h>
#include <SparkFun_SPI_SerialFlash.h>
#include <SD.h>
#include <SPI.h>


typedef union
{
  float value;
  byte array[4];
}
specialFloatT;

class Memory {

  private:
    const byte PIN_FLASH_CS = 6;
    
    const int PIN_FLASH_IO2 = 2;
    const int PIN_FLASH_IO3 = 3; 
    const int PACKET_SIZE = 32;

    SFE_SPI_FLASH flash;
    float currentMemoryPosition;

  public:
    void write_data(specialFloatT* data);
    void print_data_to_serial();
    void dump_data_to_sd_card(SDClass SD);
    void erase_data();
    SFE_SPI_FLASH get_flash();

    Memory();
};

#endif