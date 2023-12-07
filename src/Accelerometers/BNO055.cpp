#include "BNO055.h"

BNO055::BNO055(int address, TwoWire *wire) : Sensor(address, wire) {
  this->acc_rate = 16; // +-16G
  this->gyr_rate = 250; // +-250 dps
  this->mag_rate = 30; // 30Hz

  this->acc_scale = 1.0;    // LSB/mg
  this->gyr_scale = 16.0;   // LSB/deg/s
  this->mag_scale = 1.0;    // LSB/uT

  this->angle_x = 0.0;
  this->angle_y = 0.0;
  this->angle_z = 0.0;

  this->lastReading = 0.0;
}

void BNO055::begin() {
  write(PAGE_ID_REG, 0x00); // Switch to page 0
  write(OPR_MODE_REG, 0x07); // Set to AMG mode

  // Switch the X, and Y axis
  // Writing 00 10 00 01
  write(AXIS_MAP_CONFIG_REG, 0x21);

  // // Flip the X, and Y axis
  // // Writing 00 00 01 00
  // write(AXIS_MAP_SIGN_REG, 0x04);

  // Configure the accelerometer
  // +-16g range, 62.5Hz data rate, normal power mode
  // 000 011 11
  write(ACC_CONFIG_REG, 0x0F);
  

  // Configure the gyroscope
  // +-250 dps range, 47Hz data rate, normal power mode
  // Config 0: 00 011 100
  write(GYR_CONFIG_0_REG, 0x07);
  // Config 1: 00 000 000
  write(GYR_CONFIG_1_REG, 0x00);

  // Configure the magnetometer
  // 30Hz data rate, normal power mode
  // 0 00 01 111
  write(MAG_CONFIG_REG, 0x1F);
}

void BNO055::get_data(specialFloatT* data) {
 update_sensor();

  data[0].value = acc_x;
  data[1].value = acc_y;
  data[2].value = acc_z;

  data[3].value = gyr_x;
  data[4].value = gyr_y;
  data[5].value = gyr_z;

  data[6].value = mag_x;
  data[7].value = mag_y;
  data[8].value = mag_z;

  data[9].value  = angle_x;
  data[10].value = angle_y;
  data[11].value = angle_z;
}

void BNO055::update_sensor() {
  switch_page(0);
  
  /**
   * At some point we should make sure we are only reading the data
   * at the correct interval according to the configured data rates.
   * Could be done with 3 floats that store the last time the data was read
   * 
   * Also need to implement our own sensor fusion to get the roll, pitch, and yaw -- Very Basic Implementation, but should work
   * Also convert magnetometer data to compass heading?
   * 
   * This should work scale values might need to be changed, but should work.
  */
  
  // Read the accelerometer
  acc_x = read_float(ACC_DATA_X_LSB, acc_scale);
  acc_y = read_float(ACC_DATA_Y_LSB, acc_scale);
  acc_z = read_float(ACC_DATA_Z_LSB, acc_scale);

  // Read the gyroscope
  gyr_x = read_float(GYR_DATA_X_LSB, gyr_scale);
  gyr_y = read_float(GYR_DATA_Y_LSB, gyr_scale);
  gyr_z = read_float(GYR_DATA_Z_LSB, gyr_scale);

  // Read the magnetometer
  mag_x = read_float(MAG_DATA_X_LSB, mag_scale);
  mag_y = read_float(MAG_DATA_Y_LSB, mag_scale);
  mag_z = read_float(MAG_DATA_Z_LSB, mag_scale);

  // Calulate the roll, pitch, and yaw
  if (lastReading == 0.0) { // Should catch the first time this is called
    angle_x = 0.0;
    angle_y = 0.0;
    angle_z = 0.0;
  } else {
    float dt = (millis() - lastReading) / 1000.0;

    // Calculate the roll
    angle_x += gyr_x * dt;

    // Calculate the pitch
    angle_y += gyr_y * dt;

    // Calculate the yaw
    angle_z += gyr_z * dt;
  }

  lastReading = millis();
}

void BNO055::switch_page(int page) {
  if (page == 0) {
    write(PAGE_ID_REG, 0x00);
    return;
  }

  write(PAGE_ID_REG, 0x01);
}

void BNO055::calibrate() {
  Serial.println("Calibration not implemented yet ¯\\_(ツ)_/¯");
}