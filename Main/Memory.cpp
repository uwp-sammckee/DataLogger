// Memory.cpp
#include "Memory.h"

Memory::Memory(){
  pinMode(2, OUTPUT); digitalWrite(2, HIGH);
  pinMode(3, OUTPUT); digitalWrite(3, HIGH);

  flash.enableDebugging();

  if (flash.begin(PIN_FLASH_CS) == false)
  {
    Serial.println(F("SPI Flash not detected. Check wiring. Maybe you need to pull up WP/IO2 and HOLD/IO3? Freezing..."));
    while (1);
  }
  
  Serial.print("Initializing SD card...");

  if (!SD.begin(PIN_SD_CARD_CS)) {
    Serial.println("Card failed, or not present");
    while (1);
  }
  Serial.println("card initialized.");
}

void Memory::write_data(specialFloatT* data) {
  uint8_t cleanedData[32] = {
                              data[0].array[0], data[0].array[1], data[0].array[2], data[0].array[3], // roll
                              data[1].array[0], data[1].array[1], data[1].array[2], data[1].array[3], // pitch
                              data[2].array[0], data[2].array[1], data[2].array[2], data[2].array[3], // yaw

                              data[3].array[0], data[3].array[1], data[3].array[2], data[3].array[3], // alitude
                              data[4].array[0], data[4].array[1], data[4].array[2], data[4].array[3], // pressure
                              data[5].array[0], data[5].array[1], data[5].array[2], data[5].array[3], // temperature

                              0xFF, 0xFF, 0xFF, 0xFF, // place holder
                              0xFF, 0xFF, 0xFF, 0xFF // place holder
                            };
  currentMemoryPosition += 32;
  flash.writeBlock(currentMemoryPosition, cleanedData, 32);
  // Serial.println(currentMemoryPosition);
}

void Memory::print_data_to_serial() {
  for (int x = 0x0000 ; x < 0x0400 ; x++) {
    if (x % 16 == 0) {
      Serial.println();
      Serial.print("0x");
      if (x < 0x100) Serial.print("0");
      if (x < 0x10) Serial.print("0");
      Serial.print(x, HEX);
      Serial.print(": ");
    }

    byte val = flash.readByte(x);
    if (val < 0x10) Serial.print("0");
    Serial.print(val, HEX);
    Serial.print(" ");
  }

  Serial.println();
}

void Memory::dump_data_to_sd_card() {
  String dataString = "";

  SD.remove("datalog.txt");
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  
  specialFloatT temp;

  for (int x = 0x0000 ; x <= 0x0400 ; x++) {
    if (x % 64 == 0){
      dataFile.print(dataString);
      dataString = "";
    }

    if (x % PACKET_SIZE == 0) {
      // Serial.println(x);
      // roll
      flash.readBlock(x, temp.array, 4);
      // Serial.println(temp.value);
      dataString += String(temp.value);
      dataString += ", ";

      // pitch
      flash.readBlock(x+4, temp.array, 4);
      // Serial.println(temp.value);
      dataString += String(temp.value);
      dataString += ", ";

      // yaw
      flash.readBlock(x+8, temp.array, 4);
      // Serial.println(temp.value);
      dataString += String(temp.value);
      dataString += ", ";
      
      // alitude
      flash.readBlock(x+12, temp.array, 4);
      // Serial.println(temp.value);
      dataString += String(temp.value);
      dataString += ", ";
      
      // pressure
      flash.readBlock(x+16, temp.array, 4);
      // Serial.println(temp.value);
      dataString += String(temp.value);
      dataString += ", ";

      // temperature
      flash.readBlock(x+20, temp.array, 4);
      // Serial.println(temp.value);
      dataString += String(temp.value);
      dataString += ", ";

      // place holder
      flash.readBlock(x+24, temp.array, 4);
      // Serial.println(temp.value);
      dataString += String(temp.value);
      dataString += ", ";

      // place holder
      flash.readBlock(x+28, temp.array, 4);
      // Serial.println(temp.value);
      dataString += String(temp.value);

      dataString += "\n";
    }
  }

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  }
  else {
    Serial.println("error opening datalog.txt");
  }
}

void Memory::erase_data(){
  flash.erase();
  currentMemoryPosition = 0.0;
}
