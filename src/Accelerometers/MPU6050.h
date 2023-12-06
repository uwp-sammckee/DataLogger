// Accelerometers/MPU6050.h
#ifndef MPU6050_h
#define MPU6050_h

#include <Arduino.h>
#include "Wire.h"
#include <MPU6050_light.h>
#include "Memory.h"

class MPU6050_helper {

  private:
    long timer;

  public:
    void get_roll_pitch_yaw(specialFloatT* data, MPU6050 mpu);
    MPU6050_helper(MPU6050 mpu);

};

#endif