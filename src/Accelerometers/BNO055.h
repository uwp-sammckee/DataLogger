// Accelerometers/BNO055.h
#ifndef BNO055_h
#define BNO055_h

#include <BNO055_support.h>
#include <Arduino.h>
#include <Wire.h>

#include "Memory.h"
#include "Sensor.hpp"

#define BNO055_ADDR 0x29

class BNO055 : Sensor {

 private:
  /* BNO055 Struct's */  
  struct bno055_t bno055;
  struct bno055_accel accel;
  struct bno055_gyro gyro;
  struct bno055_mag mag;

  /* Data */
  short int acc_x, acc_y, acc_z;
  short int gyr_x, gyr_y, gyr_z;
  short int mag_x, mag_y, mag_z;

  // Derived from the raw data
  float angle_x, angle_y, angle_z;
  float velocity_x, velocity_y, velocity_z;

  float lastReading;

  /* Functions */
  void update_sensor();

 public:
  void begin();
  void get_data(specialFloatT* data);

  BNO055();

};

/** Notes
 * Operating Mode: AMG Non-Fusion Mode (ACC, MAG, GYRO)
 * - Write 0x07 to register 0x3D OPR_MODE
 * - Why not use Sensor Fusion? Cause it can only handle up to +-4G we should use +-16G
 *   - Found Here https://community.bosch-sensortec.com/t5/MEMS-sensors-forum/BNO055-Range-in-Fusion-mode/td-p/13437
 * 
 * Acc Radius is the same as Acc Range?
 * 
 * Power Modes:
 * - Leave in normal? ask sam what he says
 * 
 * We could remap so the Z is up, and X is forward
 * - send 00000110 to register 0x41 - BNO055_AXIS_MAP_CONFIG
 * - If we need to flip any axis, we can send 00000[0,1][0,1][0,1] to register 0x42 - BNO055_AXIS_MAP_SIGN
 * 
 * Config -- Register Map Page 1
 * - Accelerometer: Range: 16G,     Bandwidth: 62.5Hz,  Units: m/s^2, ACC_Config  0x08
 * - Gyroscope:     Range: 250dps,  Bandwidth: 47Hz,    Units: dps,   GYR_Config_1 0x0B, GYR_Config_0 0x0A
 * - Magnetometer:                  Data Rate: 30Hz,    Units: uT,    MAG_Config  0x09
 * 
 * Output registers -- Register Map Page 0
 * - Accelerometer:  Accel_Data_X/Y/Z, Signed, 2 Bytes Each
 * - Gyroscope:      Gyro_Data_X/Y/Z,  Signed, 2 Bytes Each
 * - Magnetometer:   Mag_Data_X/Y/Z,   Signed, 2 Bytes Each
 * 
 * Switch Pages by settings PAGE_ID (0x07) to 0x01 for Page 1, and 0x00 for Page 0
 * 
 * Calibration
 * - Accelerometer: Place the sensor in 6 different stable positions for a few seconds each, slowly moving between positions
 * - Gyroscope:     Place the sensor in 1 stable position for a few seconds
 * - Magnetometer:   
 *   Check CALIB_STAT register (0x35) for calibration status
 *   We can save calibration profiles
*/

#endif