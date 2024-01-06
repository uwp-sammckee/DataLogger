// Memory.cpp
#include <LittleFS.h>
#include "Memory.h"

Memory::Memory(){
  flash = LittleFS_QSPIFlash();
}

bool Memory::begin() {
  Serial.println("Initializing SPI Flash...");

  if (!flash.begin()) {
    Serial.println("SPI Flash not detected.");
    return false;
  }

  flashFile = flash.open("datalog.txt", FILE_WRITE);
  if (!flashFile) {
    Serial.println("File not opened.");
    return false;
  }

  Serial.print("\n Space Used = ");
  Serial.println(flash.usedSize());

  Serial.print("Filesystem Size = ");
  Serial.println(flash.totalSize());

  Serial.println("SPI Flash Initialized");

  return true;
}

void Memory::write_header() {
  // Log to flashFile
  if (!flashFile) {
    Serial.println("File not opened.");
    while (1); 
    return;
  }

  flashFile.println("Time,AccX,AccY,AccZ,GyrX,GyrY,GyrZ,MagX,MagY,MagZ,Heading,AngleX,AngleY,AngleZ,VelX,VelY,VelZ,Temp,Press,Alti");
  flashFile.flush();
}

void Memory::write_data(specialFloatT* data) {
  // Log to flashFile
  if (!flashFile) {
    Serial.println("File not opened.");
    while (1); 
    return;
  }

  this->data = "";
  
  this->data += String(data[0].value, 4);
  this->data += ",";

  for (int i=1; i < 20; i++) {
    this->data += String(data[i].value);
    this->data += ",";
  }

  this->unlogged_data += this->data + "\n";

  if ((millis() - last_log) >= interval) {
    last_log = millis();

    start = millis();
    flashFile.write(unlogged_data.c_str(), unlogged_data.length());
    flashFile.flush();
    end = millis();

    Serial.print("Time to log: ");
    Serial.println(end - start);

    unlogged_data = "";
  }
}

void Memory::erase_data(){
  flash.lowLevelFormat();
  Serial.println("SPI Flash erased");

  flashFile = flash.open("datalog.txt", FILE_WRITE);
  if (!flashFile) {
    Serial.println("File not opened.");
    return;
  }
}

void Memory::dump_to_sd() {
  Serial.println("Not implemented");
}

void Memory::print() {
  flashFile.flush();
  flashFile = flash.open("datalog.txt", FILE_READ);

  Serial.println("Printing flashFile");
  if (flashFile) {
    Serial.println("File Found");
    while (flashFile.available()) {
      Serial.write(flashFile.read());
    }
    flashFile.close();
  } else {
    Serial.println("File not found");
  }

  Serial.println("DATA DONE");
}


