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
    
  currentMemoryPosition = 0.0;

  Serial.println("card initialized.");
}

void Memory::write_data(specialFloatT* data) {
  uint8_t cleanedData[PACKET_SIZE] = {
                              data[0].array[0], data[0].array[1], data[0].array[2], data[0].array[3], // roll
                              data[1].array[0], data[1].array[1], data[1].array[2], data[1].array[3], // pitch
                              data[2].array[0], data[2].array[1], data[2].array[2], data[2].array[3], // yaw

                              data[3].array[0], data[3].array[1], data[3].array[2], data[3].array[3], // alitude
                              data[4].array[0], data[4].array[1], data[4].array[2], data[4].array[3], // pressure
                              data[5].array[0], data[5].array[1], data[5].array[2], data[5].array[3], // temperature

                              data[6].array[0], data[6].array[1], data[6].array[2], data[6].array[3], // accX
                              data[7].array[0], data[7].array[1], data[7].array[2], data[7].array[3], // accY
                              data[8].array[0], data[8].array[1], data[8].array[2], data[8].array[3], // accZ

                              data[9].array[0], data[9].array[1], data[9].array[2], data[9].array[3], // gyroX
                              data[10].array[0], data[10].array[1], data[10].array[2], data[10].array[3], // gyroY
                              data[11].array[0], data[11].array[1], data[11].array[2], data[11].array[3], // gyroZ
                              data[12].array[0], data[12].array[1], data[12].array[2], data[12].array[3], // time
                              0xFF, 0xFF, 0xFF, 0xFF, // place holder
                              0xFF, 0xFF, 0xFF, 0xFF, // place holder
                              0xFF, 0xFF, 0xFF, 0xFF, // place holder
                            };
  currentMemoryPosition += PACKET_SIZE;
  flash.writeBlock(currentMemoryPosition, cleanedData, PACKET_SIZE);
  // Serial.println(currentMemoryPosition);
}

SFE_SPI_FLASH Memory::get_flash(){
  return flash;
}

void Memory::print_data_to_serial() {
  String dataString = "";
  
  specialFloatT temp;

  for (int x = 0x0000 ; x <= (currentMemoryPosition+PACKET_SIZE); x++) {
    if (x % PACKET_SIZE == 0){
      Serial.println(dataString);
      dataString = "";
    }

    if (x % PACKET_SIZE == 0) {
      // roll
      flash.readBlock(x, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";

      // pitch
      flash.readBlock(x+4, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";

      // yaw
      flash.readBlock(x+8, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";
      
      // alitude
      flash.readBlock(x+12, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";
      
      // pressure
      flash.readBlock(x+16, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";

      // temperature
      flash.readBlock(x+20, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";

      // accelerometer X
      flash.readBlock(x+24, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";

      // accelerometer Y
      flash.readBlock(x+28, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";


      // accelerometer Z
      flash.readBlock(x+32, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";

      // gyroscope X
      flash.readBlock(x+36, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";

      // gyroscope Y
      flash.readBlock(x+40, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";

      // gyroscope Z
      flash.readBlock(x+44, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";

      // time
      flash.readBlock(x+48, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";

      // place holder
      flash.readBlock(x+52, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";

      // place holder
      flash.readBlock(x+56, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";

      // place holder
      flash.readBlock(x+60, temp.array, 4);
      dataString += String(temp.value);
    }
  }
  Serial.println("DATA DONE");
}

void Memory::erase_data(){
  flash.erase();
  currentMemoryPosition = 0.0;
}
