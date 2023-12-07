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
                              data[0].array[0], data[0].array[1], data[0].array[2], data[0].array[3], // Time

                              data[1].array[0], data[1].array[1], data[1].array[2], data[1].array[3], // Accelerometer X
                              data[2].array[0], data[2].array[1], data[2].array[2], data[2].array[3], // Accelerometer Y
                              data[3].array[0], data[3].array[1], data[3].array[2], data[3].array[3], // Accelerometer Z

                              data[4].array[0], data[4].array[1], data[4].array[2], data[4].array[3], // Gyro X
                              data[5].array[0], data[5].array[1], data[5].array[2], data[5].array[3], // Gyro Y
                              data[6].array[0], data[6].array[1], data[6].array[2], data[6].array[3], // Gyro Z

                              data[7].array[0], data[7].array[1], data[7].array[2], data[7].array[3], // Magnetometer X
                              data[8].array[0], data[8].array[1], data[8].array[2], data[8].array[3], // Magnetometer Y
                              data[9].array[0], data[9].array[1], data[9].array[2], data[9].array[3], // Magnetometer Z
                              
                              data[10].array[0], data[10].array[1], data[10].array[2], data[10].array[3], // Angle X
                              data[11].array[0], data[11].array[1], data[11].array[2], data[11].array[3], // Angle Y
                              data[12].array[0], data[12].array[1], data[12].array[2], data[12].array[3], // Angle Z

                              data[13].array[0], data[13].array[1], data[13].array[2], data[13].array[3], // Temperature
                              data[14].array[0], data[14].array[1], data[14].array[2], data[14].array[3], // Pressure
                              data[15].array[0], data[15].array[1], data[15].array[2], data[15].array[3], // Altitude
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

  for (int x = 0x0000 ; x <= 0x10000000; x++) {
    if (x % PACKET_SIZE == 0){
      Serial.println(dataString);
      dataString = "";
    }

    if (x % PACKET_SIZE == 0) {
      // Time
      flash.readBlock(x, temp.array, 4);
      dataString += String(temp.value, 4);
      dataString += ", ";

      // Accelerometer X
      flash.readBlock(x+4, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";

      // Accelerometer Y
      flash.readBlock(x+8, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";
      
      // Accelerometer Z
      flash.readBlock(x+12, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";
      
      // Gyro X
      flash.readBlock(x+16, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";

      // Gyro Y
      flash.readBlock(x+20, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";

      // Gyro Z
      flash.readBlock(x+24, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";

      // Magnetometer X
      flash.readBlock(x+28, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";


      // Magnetometer Y
      flash.readBlock(x+32, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";

      // Magnetometer Z
      flash.readBlock(x+36, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";

      // Angle X
      flash.readBlock(x+40, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";

      // Angle Y
      flash.readBlock(x+44, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";

      // Angle Z
      flash.readBlock(x+48, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";

      // Temperature
      flash.readBlock(x+52, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";

      // Pressure
      flash.readBlock(x+56, temp.array, 4);
      dataString += String(temp.value);
      dataString += ", ";

      // Altitude
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
