// Accelerometer.cpp
#include "Accelerometer.h"



Accelerometer::Accelerometer(MPU6050 mpu) {
  // Serial.println("Start of Accelerometer");
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
  // float dt = (millis() - lastReading);

  counter++;

  if (counter % 10 == 0){
    counter = 0;
    last10XAcceleration = 0;
    last10XAcceleration = totalAccelerationLast10 / 10;
    totalAccelerationLast10 = 0;
  }


  mpu.update();

  // Serial.println("getAngleX(): "+String(mpu.getAngleX()));
  data[0].value = mpu.getAngleX();
  data[1].value = mpu.getAngleY();
  data[2].value = mpu.getAngleZ();

  data[6].value = mpu.getAccX();
  data[7].value = mpu.getAccY();
  data[8].value = mpu.getAccZ();

  data[9].value = mpu.getGyroX();
  data[10].value = mpu.getGyroY();
  data[11].value = mpu.getGyroZ();

  totalAccelerationLast10 += data[6].value;
  data[14].value = last10XAcceleration;
  data[15].value = totalAccelerationLast10;
  // x_velocity += data[6].value * dt;
  // y_velocity += data[7].value * dt;
  // z_velocity += data[8].value * dt;

  for (int i=0; i < 16; i++) {
    Serial.print(data[i].value);
    Serial.print(", ");
  }

  Serial.println();

  lastReading = millis();
}