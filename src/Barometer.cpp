// Barometer.cpp
#include "Barometer.h"

Barometer::Barometer() {

}

void Barometer::begin(int overSampleRate) {
  byte barometerSettings = 0x00;

  if (overSampleRate > 7)
    overSampleRate = 7;

  barometerSettings |= (1 << 7);              // Set the 7th bit to altimeter mode
  barometerSettings |= (overSampleRate << 3); // Set the oversample bits to max over sample

  Wire.begin();
  Wire.beginTransmission(BAROMETER);

  Wire.write(CTRL_REG1);   // CTRL_REG1
  Wire.write(barometerSettings);

  Wire.endTransmission(true);
}

void Barometer::get_alt_pres_temp(specialFloatT* data) {
  // === Read Barometer Data === //
  update_sensor();

  // Get altitude and pressure
  Wire.beginTransmission(BAROMETER);
  Wire.write(OUT_P_MSB);

  Wire.endTransmission(false);

  if (Wire.requestFrom(BAROMETER, 3) != 3) {
    data[3].value = 0.0f;
    data[4].value = 0.0f;
    data[5].value = 0.0f;
  }

  byte msb, csb, lsb; // 3 Temp variables

  msb = Wire.read(); // Read 1 byte of data
  csb = Wire.read(); // Read 1 byte of data
  lsb = Wire.read(); // Read 1 byte of data

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

  // Get Tempurature
  Wire.beginTransmission(BAROMETER);
  Wire.write(OUT_T_MSB);
  
  Wire.endTransmission(false);

  if (Wire.requestFrom(BAROMETER, 2) != 2) {
    data[5].value = 0.0f;
  }

  msb = Wire.read(); // Read 1 byte of data
  lsb = Wire.read(); // Read 1 byte of data

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

void Barometer::update_sensor() {
  Wire.beginTransmission(BAROMETER);

  Wire.write(CTRL_REG1);
  Wire.endTransmission(false);

  // Get the current settings
  Wire.requestFrom(BAROMETER, 1);

  byte tempSetting = Wire.read();
  tempSetting &= ~(1<<1); // Turn off OST

  Wire.write(CTRL_REG1);
  Wire.write(tempSetting); // Send new settings

  Wire.endTransmission(false);

  Wire.write(CTRL_REG1);
  Wire.endTransmission(false);

  // Get the updated settings
  Wire.requestFrom(BAROMETER, 1);

  tempSetting = Wire.read();
  tempSetting |= (1<<1); // Turn on OST

  Wire.beginTransmission(BAROMETER);

  Wire.write(CTRL_REG1);
  Wire.write(tempSetting); // Send new settings

  Wire.endTransmission(true);
}