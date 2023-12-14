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

    float lastReading;
    float currentReading;

    float x_velocity;
    float y_velocity;
    float z_velocity;

    int counter;
    float totalAccelerationLast10;
    float last10XAcceleration;
    float testt; // DO NOT REMOVE - CODE DOESN'T WORK OTHERWISE!

  public:
    void get_roll_pitch_yaw(specialFloatT* data, MPU6050 mpu);
    Accelerometer(MPU6050 mpu);

    float get_last_10_acceleration_avg(){return 0.0;} //last10XAcceleration;}

    float get_x_velocity(){return x_velocity;}
    float get_y_velocity(){return y_velocity;}
    float get_z_velocity(){return z_velocity;}
    



};

#endif