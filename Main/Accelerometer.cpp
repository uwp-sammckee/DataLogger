// Accelerometer.cpp
#include "Accelerometer.h"



Accelerometer::Accelerometer(MPU6050 mpu) {
  Serial.println("Start of Accelerometer");
  // Wire.begin();
  // byte status = mpu.begin();
  // Serial.print(F("MPU6050 status: "));
  // Serial.println(status);
  // // while(status!=0){ } // stop everything if could not connect to MPU6050
  
  // // Serial.println(F("Calculating offsets, do not move MPU6050"));
  // // delay(1000);
  // // mpu.calcOffsets(true,true); // gyro and accelero
  // // Serial.println("Done!\n");
}

void Accelerometer::get_roll_pitch_yaw(specialFloatT *data, MPU6050 mpu) {
  mpu.update();
  data[0].value = mpu.getAngleX();
  data[1].value = mpu.getAngleY();
  data[2].value = mpu.getAngleZ();
}