// Accelerometer.h
#ifndef ACCELEROMETER_h
#define ACCELEROMETER_h

#include <Arduino.h>
#include <Wire.h>
#include "Memory.h"

class Accelerometer {

  private:
    const int MPU = 0x68; // MPU6050 I2C address

    float AccX, AccY, AccZ;
    float GyroX, GyroY, GyroZ;
    float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
    float roll, pitch, yaw;
    float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
    float elapsedTime, currentTime, previousTime;
    int C = 0;

  public:
    void get_roll_pitch_yaw(specialFloatT* data);
    void calculate_IMU_error();
    Accelerometer();

};

#endif