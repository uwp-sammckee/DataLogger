#include "BNO055.h"

#include "ColorLED.hpp"

BNO055::BNO055() : Sensor(BNO055_ADDR, &Wire) {
  this->angleX = 0.0;
  this->angleY = 0.0;
  this->angleZ = 0.0;

  this->velocityX = 0.0f;
  this->velocityY = 0.0f;
  this->velocityZ = 0.0f;

  this->accX = 0.0f;
  this->accY = 0.0f;
  this->accZ = 0.0f;

  this->gyroX = 0.0f;
  this->gyroY = 0.0f;
  this->gyroZ = 0.0f;

  this->magX = 0.0f;
  this->magY = 0.0f;
  this->magZ = 0.0f;

  this->heading = 0.0f;

  // Accelerometer
  accSettings = 0b00000000;
  switch (accFreq) { // Set the frequency
    case FREQ_7_81:   accSettings |= 0b00000000; break;
    case FREQ_15_63:  accSettings |= 0b00000100; break;
    case FREQ_31_25:  accSettings |= 0b00001000; break;
    case FREQ_62_5:   accSettings |= 0b00001100; break;
    case FREQ_125:    accSettings |= 0b00010000; break;
    case FREQ_250:    accSettings |= 0b00010100; break;
    case FREQ_500:    accSettings |= 0b00011000; break;
    case FREQ_1000:   accSettings |= 0b00011100; break;
  }

  switch (accRange) { // Set the range
    case RANGE_2:  accSettings |= 0b00000000; ACC_SCALE = 4.0 / 32768.0;  break;
    case RANGE_4:  accSettings |= 0b00000001; ACC_SCALE = 8.0 / 32768.0;  break;
    case RANGE_8:  accSettings |= 0b00000010; ACC_SCALE = 16.0 / 32768.0; break;
    case RANGE_16: accSettings |= 0b00000011; ACC_SCALE = 32.0 / 32768.0; break;
  }

  // Gyroscope
  gyroSettings = 0b00000010;
  switch (gyroFreq) { // Set the frequency
    case FREQ_523: gyroSettings |= 0b00000000; break;
    case FREQ_230: gyroSettings |= 0b00001000; break;
    case FREQ_116: gyroSettings |= 0b00010000; break;
    case FREQ_47:  gyroSettings |= 0b00011000; break;
    case FREQ_23:  gyroSettings |= 0b00100000; break;
    case FREQ_12:  gyroSettings |= 0b00101000; break;
    case FREQ_64:  gyroSettings |= 0b00110000; break;
    case FREQ_32:  gyroSettings |= 0b00111000; break;
  }

  switch (gyroRange) { // Set the range
    case RANGE_2000: gyroSettings |= 0b00000000; GYRO_SCALE = 4000.0 / 32768.0; break;
    case RANGE_1000: gyroSettings |= 0b00000001; GYRO_SCALE = 2000.0 / 32768.0; break;
    case RANGE_500:  gyroSettings |= 0b00000010; GYRO_SCALE = 1000.0 / 32768.0; break;
    case RANGE_250:  gyroSettings |= 0b00000011; GYRO_SCALE = 500.0 / 32768.0;  break;
    case RANGE_125:  gyroSettings |= 0b00000100; GYRO_SCALE = 256.0 / 32768.0;  break;
  }

  // Magnetometer
  magSettings = 0b00001000;
  switch (magFreq) { // Set the frequency
    case FREQ_2:  magSettings |= 0b00000000; break;
    case FREQ_6:  magSettings |= 0b00000001; break;
    case FREQ_8:  magSettings |= 0b00000010; break;
    case FREQ_10: magSettings |= 0b00000011; break;
    case FREQ_15: magSettings |= 0b00000100; break;
    case FREQ_20: magSettings |= 0b00000101; break;
    case FREQ_25: magSettings |= 0b00000110; break;
    case FREQ_30: magSettings |= 0b00000111; break;
  }
  
  MAG_SCALE = 8.0 / 32768.0;
}

bool BNO055::begin() {
  byte chip_id[1];
  read(CHIP_ID_REG, chip_id);

  if (chip_id[0] != 0xA0) {
    Serial.println("BNO055 Chip ID incorrect");
    Serial.print("Chip ID: ");
    Serial.println(chip_id[0], HEX);
    return false;
  }
  
  delay(100);

  // Put BN0055 into config mode
  write(OPR_MODE_REG, 0x00);
  delay(25);

  // Change to page 1
  write(PAGE_ID_REG, 0x01);

  // Set the Accelerometer config
  write(ACC_CONFIG_REG, accSettings);

  // Set the Gyro config 0
  write(GYR_CONFIG_0_REG, gyroSettings);

  // Set the Gyro config 1 to
  write(GYR_CONFIG_1_REG, 0b00000000);

  // Set the Magnetometer config to
  write(MAG_CONFIG_REG, magSettings);

  // Change to page 0
  write(PAGE_ID_REG, 0x00);

  // Set Power Mode to 0x00 Normal Mode
  write(PWR_MODE_REG, 0x00);

  // Set Units to, 10000000
  write(UNIT_SEL_REG, 0b10000000);

  // Set the Axis Map to switch the X, and Z axis. Set to 00000110
  write(AXIS_MAP_CONFIG, 0b00000110);

  // Set the Axis Map Sign to flip all of the axis. Set to 00000111
  write(AXIS_MAP_SIGN, 0b00000111);
  
  // Set the Operating mode to, NDOF 00001100: 0x0C
  write(OPR_MODE_REG, 0b00001100);

  delay(25);
  update_sensor();

  ColorLED::show_red();
  Serial.println("Calibration the accelerometer");

  // Make sure we get a good calibration at least 5 times
  for (int i=0; i<5; i++) {
    // While not calibrated
    while (this->get_calibration_status(true) != 255) {
      i = 0; // Reset the counter if we don't have a good calibration
      delay(50);

      // Check if the button is pressed, if it is, cancel the calibration
      if (digitalRead(14) == HIGH) {
        Serial.println("Calibration canceled");
        i = 5;
        break;
      }
    }
  }

  Serial.println("Calibration finished\n");

  delay(20);

  // Put BN0055 into config mode
  write(OPR_MODE_REG, 0x00);
  delay(25);

  // Set the Operating mode to, AMG 00000111
  write(OPR_MODE_REG, 0b00000111);
  delay(20);

  return true;
}

void BNO055::get_data(Data *data) {
  update_sensor();

  data->accX.value = accX;
  data->accY.value = accY;
  data->accZ.value = accZ;
  
  data->gyrX.value = gyroX;
  data->gyrY.value = gyroY;
  data->gyrZ.value = gyroZ;

  data->magX.value = magX;
  data->magY.value = magY;
  data->magZ.value = magZ;
  data->heading.value = heading;

  data->gyr_roll.value  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
  data->gyr_pitch.value = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;

  data->gyr_dt.value = gyroDT;
  data->gyr_dt.value = accDT;
}

void BNO055::update_sensor() {
  byte raw_data[6];

  // Check if it is time to read the accelerometer
  if ((millis() - accLast) >= accInterval) {
    // The accelerometer data is stored in the first 6 bytes
    read(ACC_DATA_X_LSB_REG, raw_data, 6);

    this->accX = static_cast<float>((int16_t)(raw_data[0] | (raw_data[1] << 8))) / 100.0;
    this->accY = static_cast<float>((int16_t)(raw_data[2] | (raw_data[3] << 8))) / 100.0;
    this->accZ = static_cast<float>((int16_t)(raw_data[4] | (raw_data[5] << 8))) / 100.0;

    accDT = ((millis() - accLast)) / 1000.0;
    accLast = millis(); // Reset the timer
  }

  // Check if it is time to read the gyroscope
  if ((millis() - gyroFreq) >= gyroInterval) {
    // The gyroscope data is stored in the next 6 bytes
    read(GYR_DATA_X_LSB_REG, raw_data, 6);

    this->gyroX = (float) ((int16_t)(raw_data[0] | ((int16_t)raw_data[1] << 8))) / 900.0;
    this->gyroY = (float) ((int16_t)(raw_data[2] | ((int16_t)raw_data[3] << 8))) / 900.0;
    this->gyroZ = (float) ((int16_t)(raw_data[4] | ((int16_t)raw_data[5] << 8))) / 900.0;

    gyroDT = ((millis() - gyroLast)) / 1000.0;
    gyroLast = millis(); // Reset the timer
  }

  // Check if it is time to read the magnetometer
  if ((millis() - magLast) >= magInterval) {
    // The magnetometer data is stored in the next 6 bytes
    read(MAG_DATA_X_LSB_REG, raw_data, 6);
    
    this->magX = (float) ((int16_t)(raw_data[0] | ((int16_t)raw_data[1] << 8))) / 1.6;
    this->magY = (float) ((int16_t)(raw_data[2] | ((int16_t)raw_data[3] << 8))) / 1.6;
    this->magZ = (float) ((int16_t)(raw_data[4] | ((int16_t)raw_data[5] << 8))) / 1.6;

    // https://arduino.stackexchange.com/a/57297
    this->heading = atan2(magY, magX) * 180 / M_PI;
    // We need a more complex formula to calculate the heading
    // because the rocket will be rotating in all 3 axis
  }
}

int BNO055::get_calibration_status(bool print) {
  byte calib_status[1];
  read(CALIB_STAT_REG, calib_status);

  if (print) {
    uint8_t system  = (calib_status[0] >> 6) & 0x03;  // Bits 7-6 represent Sys calibration
    uint8_t gyro    = (calib_status[0] >> 4) & 0x03;  // Bits 5-4 represent Gyro calibration
    uint8_t accel   = (calib_status[0] >> 2) & 0x03;  // Bits 3-2 represent Accel calibration
    uint8_t mag     = calib_status[0] & 0x03;         // Bits 1-0 represent Mag calibration

    if (system != 3) {
      Serial.print("System: ");
      Serial.print(system, BIN);
      Serial.print(", ");
    }

    if (gyro != 3) {
      Serial.print("Gyro: ");
      Serial.print(gyro, BIN);
      Serial.print(", ");
    }

    if (accel != 3) {
      Serial.print("Accel: ");
      Serial.print(accel, BIN);
      Serial.print(", ");
    }

    if (mag != 3) {
      Serial.print("Mag: ");
      Serial.print(mag, BIN);
    }

    if (calib_status[0] != 255)
      Serial.println(" Are not Fully calibrated!");

  }

  return calib_status[0];
}
