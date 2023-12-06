// Barometers/MPL3115A2.cpp
#include "MPL3115A2.h"

MPL3115A2::MPL3115A2(int address, TwoWire *wire) : Sensor(address, wire) {

}

void MPL3115A2::begin(int overSampleRate) {
  byte barometerSettings = 0x00;

  if (overSampleRate > 7)
    overSampleRate = 7;

  barometerSettings |= (1 << 7);              // Set the 7th bit to altimeter mode
  barometerSettings |= (overSampleRate << 3); // Set the oversample bits to max over sample

  write(CTRL_REG1, barometerSettings);
}

void MPL3115A2::get_data(specialFloatT* data) {
  // === Read MPL3115A2 Data === //
  update_sensor();

  byte bytes[3];
  // Get altitude and pressure
  read(OUT_P_MSB, bytes, 3);

  byte msb = bytes[0]; // Most significant byte
  byte csb = bytes[1]; // Center byte
  byte lsb = bytes[2]; // Least significant byte

  // Get altitude
	float altitudeDecimal = (lsb >> 4)/16.0; // 

  data[3].value = (float)( (msb << 8) | csb) + altitudeDecimal;

  // Get pressure
  long pressureWhole = (long)msb<<16 | (long)csb<<8 | (long)lsb;
  pressureWhole >>= 6;

  lsb &= 4;
  lsb >>= 4;
  float pressureDecimal = (float)lsb/4.0;

  data[4].value = (float)pressureWhole + pressureDecimal;

  // Get tempurature
  read(OUT_T_MSB, bytes, 2);

  msb = bytes[0]; // Most significant byte
  lsb = bytes[1]; // Least significant byte

  uint16_t tmp = 0;
  bool negative = false;

  // Check to see if 2's compliment
  if (msb > 0x7F) {
    tmp = ~((msb << 8) + lsb) + 1; // 2's complement
    msb = tmp >> 8;
    lsb = tmp * 0x00F0;
    negative = true;
  }

  float tempDecimal = (lsb >> 4) / 16.0;
  float temp = (float)(msb + tempDecimal);

  if (negative) temp = 0 - temp;

  data[5].value = temp;
}

void MPL3115A2::update_sensor() {
  // Get the current settings
  byte tempSetting;
  read(CTRL_REG1, &tempSetting, 1);

  tempSetting &= ~(1<<1); // Turn off OST

  // Send new settings
  write(CTRL_REG1, tempSetting);

  read(CTRL_REG1, &tempSetting, 1);
  tempSetting |= (1<<1); // Turn on OST

  // Send new settings
  write(CTRL_REG1, tempSetting);
}