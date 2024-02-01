// Memory.cpp
#include "Memory.h"

Memory::Memory() :  flashTransport(FLASH_CS, &SPI2),
                    flash(&flashTransport) 
{
  address = 0;
}

bool Memory::begin() {
  if (!flash.begin(&flashIC, 1)) {
    Serial.println("Flash failed to start");
    // return false;
  }

  Serial.print("JEDEC ID 0x");
  Serial.println(flash.getJEDECID(), HEX);
  Serial.print("Flash size: ");
  Serial.println(flash.size());

  Serial.println("Flash started");
  
  return true;
}

void Memory::write_data(Data *data) {
  flash.writeBuffer(address, data->getDatapoints(), size*4);

  address += size*4;
}

void Memory::erase_data(){
  flash.eraseChip();
}

void Memory::dump_to_sd() {
}

void Memory::print(Data *data) {
  Serial.println("Printing data");
  for (int64_t i=0; i < 0x10000; i += size*4) {
    flash.readBuffer(i, buf, size*4);
    data->read(buf);
  }
  Serial.println("done");
}


