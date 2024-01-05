// Accelerometers/BNO055.h
#ifndef BNO055_h
#define BNO055_h

#include <Arduino.h>
#include <Wire.h>

#include "Memory.h"
#include "Sensor.hpp"

#define BNO055_ADDR 0x29

class BNO055 : Sensor {

 private:
  // Registers
  const int CHIP_ID_REG     = 0x00;
  const int OPR_MODE_REG    = 0x3D;
  const int PAGE_ID_REG     = 0x07;
  const int CALIB_STAT_REG  = 0x35;

  // Config Registers
  const int ACC_CONFIG_REG    = 0x08;
  const int GYR_CONFIG_0_REG  = 0x0A;
  const int GYR_CONFIG_1_REG  = 0x0B;
  const int MAG_CONFIG_REG    = 0x09;
  const int PWR_MODE_REG      = 0x3E;
  const int UNIT_SEL_REG      = 0x3B;

  // Data Registers
  const int ACC_DATA_X_LSB_REG = 0x08;
  const int GYR_DATA_X_LSB_REG = 0x14;
  const int MAG_DATA_X_LSB_REG = 0x0E;

  // Config
  const float ACC_SCALE   = 32.0 / 32768.0;     // Scaling factor for accelerometer (±16 g)
  const float GYRO_SCALE  = 500.0 / 32768.0;  // Scaling factor for gyroscope (±500 dps)
  const float MAG_SCALE   = 4.0 / 32768.0;    // Scaling factor for magnetometer (±4 µT)

  // Data
  float acc_x,  acc_y,  acc_z;
  float gyro_x, gyro_y, gyro_z;
  float mag_x,  mag_y,  mag_z;

  // Timing
  float acc_freq = 31.25;                           // the frequency of the accelerometer in hz
  unsigned long acc_interval = acc_freq / 1000.0;   // the interval between readings in ms
  unsigned long acc_last = 0.0;                     // the time since the last reading in ms

  float gyro_freq = 47.0;                           // the frequency of the gyro in hz
  unsigned long gyro_interval = gyro_freq / 1000.0; // the interval between readings in ms
  unsigned long gyro_last = 0.0;                    // the time since the last reading in ms

  float mag_freq = 30.0;                            // the frequency of the magnetometer in hz
  unsigned long mag_interval = mag_freq / 1000.0;   // the interval between readings in ms
  unsigned long mag_last = 0.0;                     // the time since the last reading in ms

  // Data derived from the raw data
  float angle_x, angle_y, angle_z;
  float velocity_x, velocity_y, velocity_z;
  float heading;
  float dt;

  /* Functions */
  void update_sensor();

 public:
  bool begin();
  void get_data(specialFloatT* data);
  void reset();

  int get_calibration_status(bool print);

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
 * - Leave in normal
 * 
 * We could remap so the Z is up, and X is forward
 * - send 00000110 to register 0x41 - BNO055_AXIS_MAP_CONFIG
 * - If we need to flip any axis, we can send 00000[0,1][0,1][0,1] to register 0x42 - BNO055_AXIS_MAP_SIGN
 * 
 * Config -- Register Map Page 1
 * - Accelerometer: Range: 16G,     Bandwidth: 31.25Hz, Units: m/s^2, ACC_Config  0x0B
 * - Gyroscope:     Range: 500dps,  Bandwidth: 47Hz,    Units: dps,   GYR_Config_1 0x1A, GYR_Config_0 0x00
 * - Magnetometer:                  Data Rate: 30Hz,    Units: uT,    MAG_Config  0xFF
 * - Units:         Acceleration: m/s^2, Gyroscope: dps, Magnetometer: uT : UNIT_SEL 0x80
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
 *   We can save calibration profiles?
*/

#endif