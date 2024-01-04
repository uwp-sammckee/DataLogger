#include "BNO055.h"

BNO055::BNO055() : Sensor(BNO055_ADDR, &Wire) {
  this->angle_x = 0.0;
  this->angle_y = 0.0;
  this->angle_z = 0.0;

  this->acc_x = 0.0f;
  this->acc_y = 0.0f;
  this->acc_z = 0.0f;

  this->gyr_x = 0.0f;
  this->gyr_y = 0.0f;
  this->gyr_z = 0.0f;

  this->mag_x = 0.0f;
  this->mag_y = 0.0f;
  this->mag_z = 0.0f;

  this->lastReading = 0.0;
}

bool BNO055::begin() {
  byte chip_id;
  read(BNO055_CHIP_ID_ADDR, &chip_id);

  if (chip_id != 0xA0) {
    return false;
  }

  BNO_Init(&bno055);
  bno055_set_operation_mode(OPERATION_MODE_AMG);
  
  delay(100);

  return true;
}

void BNO055::get_data(specialFloatT* data) {
 update_sensor();

  data[1].value = float(accel.x) / 16.0;
  data[2].value = float(accel.y) / 16.0;
  data[3].value = float(accel.z) / 16.0;
  
  data[4].value = gyro.x / 16.0;
  data[5].value = gyro.y / 16.0;
  data[6].value = gyro.z / 16.0;

  data[7].value = mag.x;
  data[8].value = mag.y;
  data[9].value = mag.z;

  data[10].value = angle_x;
  data[11].value = angle_y;
  data[12].value = angle_z;

  data[13].value = velocity_x;
  data[14].value = velocity_y;
  data[15].value = velocity_z;
}

void BNO055::update_sensor() {
  // bno055_read_accel_x(&acc_x);
  // bno055_read_accel_y(&acc_y);
  // bno055_read_accel_z(&acc_z);

  bno055_read_accel_xyz(&accel);
  bno055_read_gyro_xyz(&gyro);
  bno055_read_mag_xyz(&mag);
  

  // Calulate the derived values
  if (lastReading != 0.0) { 
    // This will stop it from being called the first time
    
    float dt = (millis() - lastReading) / 1000.0;

    // Calculate the roll, pitch, and yaw
    angle_x += gyro.x * dt;
    angle_y += gyro.y * dt;
    angle_z += gyro.z * dt;

    // Calculate the velocity
    velocity_x += accel.x * dt;
    velocity_y += accel.y * dt;
    velocity_z += accel.z * dt;
  }

  lastReading = millis();
}
