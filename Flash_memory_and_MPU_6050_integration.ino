typedef union
{
  float value;
  byte array[4];
}
special_float_t;

typedef union
{
  uint32_t value;
  byte array[4];
}
current_position_t;


#include <SPI.h>
#include <SD.h>
#include <SparkFun_SPI_SerialFlash.h> //Click here to get the library: http://librarymanager/All#SparkFun_SPI_SerialFlash
#include <Adafruit_MPL3115A2.h>
#include <Wire.h>

SFE_SPI_FLASH myFlash;
const byte PIN_FLASH_CS = 6; // Change this to match the Chip Select pin on your board
const int PIN_SD_CARD_CS = 7;
const int buzzer = 4;
int redPin = 10;
int greenPin = 9;
int bluePin = 8;

const int recordDataPin = 5;
bool recordData = false;
int recordDataState = 0; 

const int dumpDataPin = A2;
bool dumpData = false;
int dumpDataState = 0; 

const int eraseDataPin = A1;
bool eraseData = false;
int eraseDataState = 0; 

const int packet_size = 32; // size of roll, pitch, yaw, altimeter data
const int start_address = 1024;
const int position_address = 0; //0 - 32

const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;
int c = 0;

Adafruit_MPL3115A2 baro;

special_float_t data[6];
float lastMemoryPos;


void setup() {
  Serial.begin(19200);
  // Flash memory setup
  pinMode(2, OUTPUT); digitalWrite(2, HIGH); // Uncomment this line to use pin 2 to pull up (e.g.) the WP/IO2 pin
  pinMode(3, OUTPUT); digitalWrite(3, HIGH); // Uncomment this line to use pin 3 to pull up (e.g.) the HOLD/IO3 pin

  myFlash.enableDebugging(); // Uncomment this line to enable helpful debug messages on Serial

  // Begin the flash using the chosen CS pin. Default to: spiPortSpeed=2000000, spiPort=SPI and spiMode=SPI_MODE0
  if (myFlash.begin(PIN_FLASH_CS) == false)
  {
    Serial.println(F("SPI Flash not detected. Check wiring. Maybe you need to pull up WP/IO2 and HOLD/IO3? Freezing..."));
    while (1);
  }

  Serial.println(F("SPI Flash detected"));

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
  
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(PIN_SD_CARD_CS)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT); 

  pinMode(recordDataPin, INPUT);

  if (!baro.begin()) {
    Serial.println("Could not find sensor. Check wiring.");
    while(1);
  }

  baro.setSeaPressure(1013.26);

  // Call this function if you need to get the IMU error values for your module
  calculate_IMU_error();

  delay(20);
  start_up_sound();
  // read_data();
}

void loop() {
  
  show_blue();
  delay(100);

  recordDataState = digitalRead(recordDataPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (recordDataState == HIGH) {
    if (recordData) {
      recordData = false;
    } else {
      recordData = true;
      delay(1000);
    }
  }

  dumpDataState = digitalRead(dumpDataPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (dumpDataState == HIGH) {
    if (dumpData) {
      dumpData = false;
    } else {
      dumpData = true;
    }
    delay(1000);
  }

  eraseDataState = digitalRead(eraseDataPin);

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
    for (int x = 0x0000 ; x < 0x0400 ; x++)
    {
      if (x % 16 == 0) {
        Serial.println();
        Serial.print("0x");
        if (x < 0x100) Serial.print("0");
        if (x < 0x10) Serial.print("0");
        Serial.print(x, HEX);
        Serial.print(": ");
      }

      byte val = myFlash.readByte(x);
      if (val < 0x10) Serial.print("0");
      Serial.print(val, HEX);
      Serial.print(" ");
    }

    Serial.println();
    }
  }

  if (recordData){
    show_yellow();
    get_roll_pitch_yaw(data);
    Serial.print(data[0].value);
    Serial.print("/");
    Serial.print(data[1].value);
    Serial.print("/");
    Serial.println(data[2].value);
    
    data[3].value = baro.getAltitude();
    data[4].value = baro.getPressure();
    data[5].value = baro.getTemperature();

    write_data(data);
  }

  if (eraseData) {
    Serial.println("Erasing entire chip");
    start_sound();
    show_red();
    myFlash.erase();
    lastMemoryPos = 0.0;
    end_sound();
    eraseData = false;
  }

  Serial.println(dumpData);
  if (dumpData) {
    start_sound();
    show_red();
    String dataString = "";

    SD.remove("datalog.txt");
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    
    special_float_t temp;

    for (int x = 0x0000 ; x <= 0x0400 ; x++) {
      if (x % 64 == 0){
        dataFile.print(dataString);
        dataString = "";
      }

      if (x % packet_size == 0) {
        // Serial.println(x);
        // roll
        myFlash.readBlock(x, temp.array, 4);
        // Serial.println(temp.value);
        dataString += String(temp.value);
        dataString += ", ";

        // pitch
        myFlash.readBlock(x+4, temp.array, 4);
        // Serial.println(temp.value);
        dataString += String(temp.value);
        dataString += ", ";

        // yaw
        myFlash.readBlock(x+8, temp.array, 4);
        // Serial.println(temp.value);
        dataString += String(temp.value);
        dataString += ", ";
        
        // alitude
        myFlash.readBlock(x+12, temp.array, 4);
        // Serial.println(temp.value);
        dataString += String(temp.value);
        dataString += ", ";
        
        // pressure
        myFlash.readBlock(x+16, temp.array, 4);
        // Serial.println(temp.value);
        dataString += String(temp.value);
        dataString += ", ";

        // temperature
        myFlash.readBlock(x+20, temp.array, 4);
        // Serial.println(temp.value);
        dataString += String(temp.value);
        dataString += ", ";

        // place holder
        myFlash.readBlock(x+24, temp.array, 4);
        // Serial.println(temp.value);
        dataString += String(temp.value);
        dataString += ", ";

        // place holder
        myFlash.readBlock(x+28, temp.array, 4);
        // Serial.println(temp.value);
        dataString += String(temp.value);

        dataString += "\n";
      }
    }

    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
    }
    else {
      Serial.println("error opening datalog.txt");
    }
    end_sound();
    dumpData = false;
  }
  
  
  delay(100);
}

void end_sound(){
  tone(buzzer, 400);
  delay(1000);
  noTone(buzzer);
}

void start_sound() {
  tone(buzzer, 1000);
  delay(200);
  noTone(buzzer);
}

void start_up_sound() {
  for (int i=100; i<=1000; i+=100){
    tone(buzzer, i);
    delay(100);
  }
  noTone(buzzer);
}

void show_red(){
  setColor(255, 0, 0);  // red
}

void show_green(){
  setColor(0, 255, 0);  // red
}

void show_blue(){
  setColor(0, 0, 255);  // red
}

void show_yellow(){
  setColor(255, 255, 0);  // red
}

void setColor(int red, int green, int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}

void write_data(special_float_t* data){

  uint8_t cleanedData[32] = {
                              data[0].array[0], data[0].array[1], data[0].array[2], data[0].array[3], // roll
                              data[1].array[0], data[1].array[1], data[1].array[2], data[1].array[3], // pitch
                              data[2].array[0], data[2].array[1], data[2].array[2], data[2].array[3], // yaw

                              data[3].array[0], data[3].array[1], data[3].array[2], data[3].array[3], // alitude
                              data[4].array[0], data[4].array[1], data[4].array[2], data[4].array[3], // pressure
                              data[5].array[0], data[5].array[1], data[5].array[2], data[5].array[3], // temperature

                              0xFF, 0xFF, 0xFF, 0xFF, // place holder
                              0xFF, 0xFF, 0xFF, 0xFF // place holder
                            };
  lastMemoryPos += 32;
  myFlash.writeBlock(lastMemoryPos, cleanedData, 32);
  Serial.println(lastMemoryPos);
}

void get_roll_pitch_yaw(special_float_t* data){
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
  while (c < 200) {
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
    c++;
  }
  //Divide the sum by 200 to get the error value
  AccErrorX = AccErrorX / 200;
  AccErrorY = AccErrorY / 200;
  c = 0;
  // Read gyro values 200 times
  while (c < 200) {
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
    c++;
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
