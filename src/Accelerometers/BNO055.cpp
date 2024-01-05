#include "BNO055.h"

#include "ColorLED.hpp"

BNO055::BNO055() : Sensor(BNO055_ADDR, &Wire) {
  this->angle_x = 0.0;
  this->angle_y = 0.0;
  this->angle_z = 0.0;

  this->acc_x = 0.0f;
  this->acc_y = 0.0f;
  this->acc_z = 0.0f;

  this->gyro_x = 0.0f;
  this->gyro_y = 0.0f;
  this->gyro_z = 0.0f;

  this->mag_x = 0.0f;
  this->mag_y = 0.0f;
  this->mag_z = 0.0f;

  this->dt = 0.0;
}

bool BNO055::begin() {
  byte chip_id[1];
  read(CHIP_ID_REG, chip_id);

  if (chip_id[0] != 0xA0) {
    Serial.println("BNO055 Chip ID incorrect");
    Serial.print("Chip ID: ");
    Serial.println(chip_id[0], HEX);
    // return false;
  }
  
  delay(100);

  // Put BN0055 into config mode
  write(OPR_MODE_REG, 0x00);
  delay(25);

  // Change to page 1
  write(PAGE_ID_REG, 0x01);

  // Set the Accelerometer config to, 0000101
  write(ACC_CONFIG_REG, 0b00001011);

  // Set the Gyro config 0 to, 00011010
  write(GYR_CONFIG_0_REG, 0b00011010);

  // Set the Gyro config 1 to, 00000000
  write(GYR_CONFIG_1_REG, 0b00000000);

  // Set the Magnetometer config to, 00001111
  write(MAG_CONFIG_REG, 0b00001111);

  // Change to page 0
  write(PAGE_ID_REG, 0x00);

  // Set Power Mode to 0x00 Normal Mode
  write(PWR_MODE_REG, 0x00);

  // Set Units to, 10000000
  write(UNIT_SEL_REG, 0b10000000);
  
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

  Serial.println("Calibration finished");

  delay(20);

  // Put BN0055 into config mode
  write(OPR_MODE_REG, 0x00);
  delay(25);

  // Set the Operating mode to, AMG 00000111
  write(OPR_MODE_REG, 0b00000111);
  delay(20);

  return true;
}

void BNO055::get_data(specialFloatT* data) {
  update_sensor();

  data[1].value = acc_x;
  data[2].value = acc_y;
  data[3].value = acc_z;
  
  data[4].value = gyro_x;
  data[5].value = gyro_y;
  data[6].value = gyro_z;

  data[7].value = mag_x;
  data[8].value = mag_y;
  data[9].value = mag_z;

  data[10].value = heading;

  data[11].value = angle_x;
  data[12].value = angle_y;
  data[13].value = angle_z;

  data[14].value = velocity_x;
  data[15].value = velocity_y;
  data[16].value = velocity_z;
}

void BNO055::update_sensor() {
  byte raw_data[6];

  // Check if it is time to read the accelerometer
  if ((millis() - acc_last) >= acc_interval) {
    // The accelerometer data is stored in the first 6 bytes
    read(ACC_DATA_X_LSB_REG, raw_data, 6);

    this->acc_x = (float) ((int16_t)(raw_data[0] | ((int16_t)raw_data[1] << 8))) / 100.0;
    this->acc_y = (float) ((int16_t)(raw_data[2] | ((int16_t)raw_data[3] << 8))) / 100.0;
    this->acc_z = (float) ((int16_t)(raw_data[4] | ((int16_t)raw_data[5] << 8))) / 100.0;

    // Calculate derived values
    dt = ((millis() - acc_last)) / 1000.0;

    // Calculate the velocity
    velocity_x += acc_x * dt;
    velocity_y += acc_y * dt;
    velocity_z += acc_z * dt;

    acc_last = millis(); // Reset the timer
  }

  // Check if it is time to read the gyroscope
  if ((millis() - gyro_freq) >= gyro_interval) {
    // The gyroscope data is stored in the next 6 bytes
    read(GYR_DATA_X_LSB_REG, raw_data, 6);

    this->gyro_x = (float) ((int16_t)(raw_data[0] | ((int16_t)raw_data[1] << 8))) / 900.0;
    this->gyro_y = (float) ((int16_t)(raw_data[2] | ((int16_t)raw_data[3] << 8))) / 900.0;
    this->gyro_z = (float) ((int16_t)(raw_data[4] | ((int16_t)raw_data[5] << 8))) / 900.0;

    // Calculate derived values
    dt = ((millis() - gyro_last)) / 1000.0;

    // Calculate the roll, pitch, and yaw
    angle_x += gyro_x * dt;
    angle_y += gyro_y * dt;
    angle_z += gyro_z * dt;

    gyro_last = millis(); // Reset the timer
  }

  // Check if it is time to read the magnetometer
  if ((millis() - mag_last) >= mag_interval) {
    // The magnetometer data is stored in the next 6 bytes
    read(MAG_DATA_X_LSB_REG, raw_data, 6);
    
    this->mag_x = (float) ((int16_t)(raw_data[0] | ((int16_t)raw_data[1] << 8))) / 1.6;
    this->mag_y = (float) ((int16_t)(raw_data[2] | ((int16_t)raw_data[3] << 8))) / 1.6;
    this->mag_z = (float) ((int16_t)(raw_data[4] | ((int16_t)raw_data[5] << 8))) / 1.6;

    // https://arduino.stackexchange.com/a/57297
    this->heading = atan2(mag_y, mag_z) * 180 / M_PI;
    // We need a more complex formula to calculate the heading
    // because the rocket will be rotating in all 3 axis
  }
}

void BNO055::reset() {
  velocity_x = 0.0;
  velocity_y = 0.0;
  velocity_z = 0.0;

  angle_x = 0.0;
  angle_y = 0.0;
  angle_z = 0.0;

  dt = 0.0;
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
