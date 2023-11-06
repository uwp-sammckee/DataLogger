#include <SPI.h>
#include <SD.h>
#include "SparkFunMPL3115A2.h"
#include <Wire.h>

#include "ColorLED.h"
#include "Buzzer.h"
#include "Memory.h"

const int PIN_RECORD_DATA = 5;
const int PIN_DUMP_DATA = A2;
const int PIN_ERASE_DATA = A1;

const int MPU = 0x68; // MPU6050 I2C address

bool recordData = false;
int recordDataState = 0; 

bool dumpData = false;
int dumpDataState = 0; 

bool eraseData = false;
int eraseDataState = 0; 

float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;
int C = 0;

MPL3115A2 baro;
SFE_SPI_FLASH flash;

Memory memory;
Buzzer buzzer;
ColorLED LED;

specialFloatT data[6];

void setup() {
  Serial.begin(19200);

  Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
  
  // Configure Accelerometer Sensitivity - Full Scale Range (default +/- 2g)
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);                  //Talk to the ACCEL_CONFIG register (1C hex)
  Wire.write(0x10);                  //Set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true);

  pinMode(buzzer.get_buzzer_pin(), OUTPUT);

  pinMode(LED.get_red_pin(), OUTPUT);
  pinMode(LED.get_blue_pin(), OUTPUT);
  pinMode(LED.get_blue_pin(), OUTPUT); 

  pinMode(PIN_RECORD_DATA, INPUT);
  pinMode(PIN_DUMP_DATA, INPUT);
  pinMode(PIN_ERASE_DATA, INPUT);

  baro.begin(); // Get sensor online
  //Configure the sensor
  baro.setModeAltimeter(); // Measure altitude above sea level in meters
  //myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa

  baro.setOversampleRate(3); // Set Oversample to the recommended 128
  baro.enableEventFlags(); // Enable all three pressure and temp event flags 

  // Call this function if you need to get the IMU error values for your module
  calculate_IMU_error();

  delay(20);
  buzzer.start_up_sound();
  // read_data();
}

void loop() {
  
  LED.show_blue();
  delay(100);

  recordDataState = digitalRead(PIN_RECORD_DATA);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (recordDataState == HIGH) {
    if (recordData) {
      recordData = false;
    } else {
      recordData = true;
      delay(1000);
    }
  }

  dumpDataState = digitalRead(PIN_DUMP_DATA);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (dumpDataState == HIGH) {
    if (dumpData) {
      dumpData = false;
    } else {
      dumpData = true;
    }
    delay(1000);
  }

  eraseDataState = digitalRead(PIN_ERASE_DATA);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (eraseDataState == HIGH) {
    if (eraseData) {
      eraseData = false;
    } else {
      eraseData = true;
    }
    delay(1000);
  }

  if (!recordData && !dumpData && !eraseData) {
    // Serial.println();
    // Serial.println("r)ead HEX values, 1k bytes");
    // Serial.println("w)rite data from MPU, 1k bytes");
    // Serial.println("e)rase entire chip");
    // Serial.println("d)ump data to SD card");
    // Serial.println();

    // while (Serial.available()) Serial.read(); //Clear the RX buffer
    // while (Serial.available() == 0); //Wait for a character

    byte choice = Serial.read();

    if (choice == 'r') {
      memory.print_data_to_serial();
    }
  }

  if (recordData){
    LED.show_yellow();
    get_roll_pitch_yaw(data);
    Serial.print(data[0].value);
    Serial.print("/");
    Serial.print(data[1].value);
    Serial.print("/");
    Serial.println(data[2].value);
    
    data[3].value = baro.readAltitudeFt();
    data[4].value = baro.readPressure();
    data[5].value = baro.readPressure();

    memory.write_data(data);
  }

  if (eraseData) {
    Serial.println("Erasing entire chip");
    buzzer.start_sound();
    LED.show_red();
    memory.erase_data();
    buzzer.end_sound();
    eraseData = false;
  }

  if (dumpData) {
    buzzer.start_sound();
    LED.show_red();
    memory.dump_data_to_sd_card();
    buzzer.end_sound();
    dumpData = false;
  }
  
  delay(100);
}

void get_roll_pitch_yaw(specialFloatT* data){
  // === Read acceleromter data === //
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  //For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
  AccX = (Wire.read() << 8 | Wire.read()) / 16384.0; // X-axis value
  AccY = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value
  AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0; // Z-axis value

  // Serial.print(AccX);
  // Serial.print("/");
  // Serial.print(AccY);
  // Serial.print("/");
  // Serial.print(AccZ);
  // Serial.println("/");

  // Calculating Roll and Pitch from the accelerometer data
  accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) + AccErrorX; // AccErrorX ~(0.58) See the calculate_IMU_error()custom function for more details
  accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) + AccErrorY; // AccErrorY ~(-1.58)
  // === Read gyroscope data === //
  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds
  Wire.beginTransmission(MPU);
  Wire.write(0x43); // Gyro data first register address 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 4 registers total, each axis value is stored in 2 registers
  GyroX = (Wire.read() << 8 | Wire.read()) / 131.0; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
  GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;
  // Correct the outputs with the calculated error values

  GyroX = GyroX + GyroErrorX; // GyroErrorX ~(-0.56)
  GyroY = GyroY + GyroErrorY; // GyroErrorY ~(2)
  GyroZ = GyroZ + GyroErrorZ; // GyroErrorZ ~ (-0.8)

  // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
  gyroAngleX = gyroAngleX + (GyroX * elapsedTime); // deg/s * s = deg
  gyroAngleY = gyroAngleY + (GyroY * elapsedTime);
  yaw =  yaw + (GyroZ * elapsedTime);
  // Complementary filter - combine acceleromter and gyro angle values
  roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
  pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;
  
  roll = gyroAngleX;
  pitch = gyroAngleY;

  //Print the values on the serial monitor
  // Serial.print(roll);
  // Serial.print("/");
  // Serial.print(pitch);
  // Serial.print("/");
  // Serial.println(yaw);

  data[0].value = roll;
  data[1].value = pitch;
  data[2].value = yaw;
}

void calculate_IMU_error() {
  // We can call this funtion in the setup section to calculate the accelerometer and gyro data error. From here we will get the error values used in the above equations printed on the Serial Monitor.
  // Note that we should place the IMU flat in order to get the proper values, so that we then can the correct values
  // Read accelerometer values 200 times
  while (C < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    AccX = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    AccY = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    // Sum all readings
    AccErrorX = AccErrorX + ((atan((AccY) / sqrt(pow((AccX), 2) + pow((AccZ), 2))) * 180 / PI));
    AccErrorY = AccErrorY + ((atan(-1 * (AccX) / sqrt(pow((AccY), 2) + pow((AccZ), 2))) * 180 / PI));
    C++;
  }
  //Divide the sum by 200 to get the error value
  AccErrorX = AccErrorX / 200;
  AccErrorY = AccErrorY / 200;
  C = 0;
  // Read gyro values 200 times
  while (C < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    GyroX = Wire.read() << 8 | Wire.read();
    GyroY = Wire.read() << 8 | Wire.read();
    GyroZ = Wire.read() << 8 | Wire.read();
    // Sum all readings
    GyroErrorX = GyroErrorX + (GyroX / 131.0);
    GyroErrorY = GyroErrorY + (GyroY / 131.0);
    GyroErrorZ = GyroErrorZ + (GyroZ / 131.0);
    C++;
  }
  //Divide the sum by 200 to get the error value
  GyroErrorX = GyroErrorX / 200;
  GyroErrorY = GyroErrorY / 200;
  GyroErrorZ = GyroErrorZ / 200;
  // Print the error values on the Serial Monitor
  Serial.print("AccErrorX: ");
  Serial.println(AccErrorX);
  Serial.print("AccErrorY: ");
  Serial.println(AccErrorY);
  Serial.print("GyroErrorX: ");
  Serial.println(GyroErrorX);
  Serial.print("GyroErrorY: ");
  Serial.println(GyroErrorY);
  Serial.print("GyroErrorZ: ");
  Serial.println(GyroErrorZ);
}