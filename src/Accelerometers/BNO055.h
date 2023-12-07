// Accelerometers/BNO055.h
#ifndef BNO055_h
#define BNO055_h

#include <Arduino.h>
#include <Wire.h>

#include "Memory.h"
#include "Sensor.hpp"

#define BNO055_ADDR 0x29

class BNO055 : public Sensor {

 private:
  /* Radius Registers */
  const int MAG_RADIUS_MSB_REG = 0x6A;
  const int MAG_RADIUS_LSB_REG = 0x69;
  const int ACC_RADIUS_MSB_REG = 0x68;
  const int ACC_RADIUS_LSB_REG = 0x67;

  /* Offset Registers */
  const int GYR_OFFSET_Z_MSB_REG = 0x66;
  const int GYR_OFFSET_Z_LSB_REG = 0x65;
  const int GYR_OFFSET_Y_MSB_REG = 0x64;
  const int GYR_OFFSET_Y_LSB_REG = 0x63;
  const int GYR_OFFSET_X_MSB_REG = 0x62;
  const int GYR_OFFSET_X_LSB_REG = 0x61;
  const int MAG_OFFSET_Z_MSB_REG = 0x60;
  const int MAG_OFFSET_Z_LSB_REG = 0x5F;
  const int MAG_OFFSET_Y_MSB_REG = 0x5E;
  const int MAG_OFFSET_Y_LSB_REG = 0x5D;
  const int MAG_OFFSET_X_MSB_REG = 0x5C;
  const int MAG_OFFSET_X_LSB_REG = 0x5B;
  const int ACC_OFFSET_Z_MSB_REG = 0x5A;
  const int ACC_OFFSET_Z_LSB_REG = 0x59;
  const int ACC_OFFSET_Y_MSB_REG = 0x58;
  const int ACC_OFFSET_Y_LSB_REG = 0x57;
  const int ACC_OFFSET_X_MSB_REG = 0x56;
  const int ACC_OFFSET_X_LSB_REG = 0x55;

  /* Raw Data Registers */
  const int GYR_DATA_Z_MSB = 0x19;
  const int GYR_DATA_Z_LSB = 0x18;
  const int GYR_DATA_Y_MSB = 0x17;
  const int GYR_DATA_Y_LSB = 0x16;
  const int GYR_DATA_X_MSB = 0x15;
  const int GYR_DATA_X_LSB = 0x14;
  const int MAG_DATA_Z_MSB = 0x13;
  const int MAG_DATA_Z_LSB = 0x12;
  const int MAG_DATA_Y_MSB = 0x11;
  const int MAG_DATA_Y_LSB = 0x10;
  const int MAG_DATA_X_MSB = 0x0F;
  const int MAG_DATA_X_LSB = 0x0E;
  const int ACC_DATA_Z_MSB = 0x0D;
  const int ACC_DATA_Z_LSB = 0x0C;
  const int ACC_DATA_Y_MSB = 0x0B;
  const int ACC_DATA_Y_LSB = 0x0A;
  const int ACC_DATA_X_MSB = 0x09;
  const int ACC_DATA_X_LSB = 0x08;

  /* Config */
  const int PAGE_ID_REG         = 0x07;
  const int OPR_MODE_REG        = 0x3D;
  const int AXIS_MAP_CONFIG_REG = 0x41;
  const int AXIS_MAP_SIGN_REG   = 0x42;
  const int GYR_CONFIG_1_REG    = 0x0B;
  const int GYR_CONFIG_0_REG    = 0x0A;
  const int MAG_CONFIG_REG      = 0x09;
  const int ACC_CONFIG_REG      = 0x08;


  /* Data */
  float acc_x, acc_y, acc_z;
  float gyr_x, gyr_y, gyr_z;
  float mag_x, mag_y, mag_z;

  float angle_x, angle_y, angle_z;

  float acc_rate, gyr_rate, mag_rate;
  float acc_scale, gyr_scale, mag_scale;

  float lastReading;

  /* Functions */
  void update_sensor();
  void switch_page(int page);

 public:
  void begin();
  void get_data(specialFloatT* data);
  void calibrate();

  BNO055(int address=BNO055_ADDR, TwoWire *wire=&Wire);

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
 * We could remap X, and Y so Y is up/down, and X is left/right, Z is forward/backward
 * - send 00100001 to register 0x41 - BNO055_AXIS_MAP_CONFIG
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