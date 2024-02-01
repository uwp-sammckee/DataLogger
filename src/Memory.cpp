// Memory.cpp
#include "Memory.h"

Memory::Memory() :  flashTransport(FLASH_CS, &SPI2),
                    flash(&flashTransport) 
{
  
}

bool Memory::begin() {
  if (!flash.begin(&flashIC, 1)) {
    Serial.println("Flash failed to start");
    return false;
  }

  Serial.print("JEDEC ID 0x");
  Serial.println(flash.getJEDECID(), HEX);
  Serial.print("Flash size: ");
  Serial.println(flash.size());

  Serial.println("Flash started");
  
  return true;
}

void Memory::write_data(Data *data) {
  flash.writeBlock(0, data->getDatapoints());
}

void Memory::erase_data(){
}

void Memory::dump_to_sd() {
}

void Memory::print() {
}


