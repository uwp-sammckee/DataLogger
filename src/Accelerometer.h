// Accelerometer.h
#ifndef ACCELEROMETER_h
#define ACCELEROMETER_h

#include <Arduino.h>
#include "Wire.h"
#include <MPU6050_light.h>
#include "Memory.h"

class Accelerometer {

  private:
    long timer;

  public:
    void get_roll_pitch_yaw(specialFloatT* data, MPU6050 mpu);
    Accelerometer(MPU6050 mpu);

};

#endif