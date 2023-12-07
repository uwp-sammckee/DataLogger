#include "Wire.h"
#include <MPU6050_light.h>

#include "ColorLED.h"
#include "Buzzer.h"
#include "Memory.h"

#include "Accelerometers/MPU6050.h"
#include "Barometers/MPL3115A2.h"

const int PIN_RECORD_DATA = 5;
const int PIN_DUMP_DATA = A2;
const int PIN_ERASE_DATA = A1;

const int PIN_SD_CARD_CS = 7;

const int MPU = 0x68; // MPU6050 I2C address

bool recordData = false;
int recordDataState = 0; 

bool dumpData = false;
int dumpDataState = 0; 

bool eraseData = false;
int eraseDataState = 0; 

MPU6050 mpu(Wire);

Buzzer buzzer;
ColorLED LED;
MPU6050_helper accelerometer(mpu);
MPL3115A2 baro;
Memory memory;

unsigned long loopLenght = 50;
unsigned long loopDelay  = 0;
unsigned long loopStart  = 0;
unsigned long timeStart  = 0;

specialFloatT data[16];

void setup() {
  Serial.begin(19200);
  Wire.begin();
  byte status = mpu.begin();
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(true,true); // gyro and accelero
  
  pinMode(buzzer.get_buzzer_pin(), OUTPUT);

  pinMode(LED.get_red_pin(), OUTPUT);
  pinMode(LED.get_blue_pin(), OUTPUT);
  pinMode(LED.get_blue_pin(), OUTPUT); 

  pinMode(PIN_RECORD_DATA, INPUT);
  pinMode(PIN_DUMP_DATA, INPUT);
  pinMode(PIN_ERASE_DATA, INPUT);

  baro.begin(); // Get sensor online

  delay(20);
  buzzer.start_up_sound();
  Serial.println("Setup finished, everything is good.");
}

void loop() {
  loopStart = millis();
  
  LED.show_blue();
  //delay(100);

  recordDataState = digitalRead(PIN_RECORD_DATA);

  // // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (recordDataState == HIGH) {
    if (recordData) {
      recordData = false;
      delay(1000);
      Serial.println("recordData false");
    } else {
      timeStart = millis();
      recordData = true;
      delay(1000);
      Serial.println("recordData true");
    }
    Serial.println("recordData: "+String(recordData));
  }

  dumpDataState = digitalRead(PIN_DUMP_DATA);

  // // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (dumpDataState == HIGH) {
    if (dumpData) {
      dumpData = false;
      delay(1000);
    } else {
      dumpData = true;
      delay(1000);
    }
    delay(1000);
  }

  eraseDataState = digitalRead(PIN_ERASE_DATA);

  // // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (eraseDataState == HIGH) {
    if (eraseData) {
      eraseData = false;
      delay(1000);
    } else {
      eraseData = true;
      delay(1000);
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

    if (choice == 'e') {
      memory.erase_data();
    }
  }

  if (recordData){
    LED.show_yellow();
    accelerometer.get_roll_pitch_yaw(data, mpu);
    Serial.println("data getAngleX(): "+String(data[0].value));
    // Serial.print("/");
    // Serial.print(data[1].value);
    // Serial.print("/");
    // Serial.println(data[2].value);
    
    baro.get_data(data);
    // Serial.print(data[3].value);
    // Serial.print("/");
    // Serial.print(data[4].value);
    // Serial.print("/");
    // Serial.println(data[5].value);

    data[1].value = (millis()-timeStart) / 1000.0f;

    Serial.println();
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
    memory.print_data_to_serial();
    buzzer.end_sound();
    dumpData = false;
  }
  
  loopDelay = loopLenght - (millis() - loopStart);
  if (loopDelay > loopLenght) loopDelay = 0;

  delay(loopDelay);
}

// void dump_data_to_sd_card() {
//   String dataString = "";
  
//   specialFloatT temp;

//   for (int x = 0x0000 ; x <= 0x0400 ; x++) {
//     if (x % 64 == 0){
//       // dataFile.print(dataString);
//       Serial.println(dataString);
//       dataString = "";
//     }

//     if (x % 32 == 0) {
//       // Serial.println(x);
//       // roll
//       memory.get_flash().readBlock(x, temp.array, 4);
//       // Serial.println(temp.value);
//       dataString += String(temp.value);
//       dataString += ", ";

//       // pitch
//       memory.get_flash().readBlock(x+4, temp.array, 4);
//       // Serial.println(temp.value);
//       dataString += String(temp.value);
//       dataString += ", ";

//       // yaw
//       memory.get_flash().readBlock(x+8, temp.array, 4);
//       // Serial.println(temp.value);
//       dataString += String(temp.value);
//       dataString += ", ";
      
//       // alitude
//       memory.get_flash().readBlock(x+12, temp.array, 4);
//       // Serial.println(temp.value);
//       dataString += String(temp.value);
//       dataString += ", ";
      
//       // pressure
//       memory.get_flash().readBlock(x+16, temp.array, 4);
//       // Serial.println(temp.value);
//       dataString += String(temp.value);
//       dataString += ", ";

//       // temperature
//       memory.get_flash().readBlock(x+20, temp.array, 4);
//       // Serial.println(temp.value);
//       dataString += String(temp.value);
//       dataString += ", ";

//       // place holder
//       memory.get_flash().readBlock(x+24, temp.array, 4);
//       // Serial.println(temp.value);
//       dataString += String(temp.value);
//       dataString += ", ";

//       // place holder
//       memory.get_flash().readBlock(x+28, temp.array, 4);
//       // Serial.println(temp.value);
//       dataString += String(temp.value);

//       dataString += "\n";
//     }
//   }

// //   // if the file is available, write to it:
// //   if (dataFile) {
// //     dataFile.println(dataString);
// //     dataFile.close();
// //   }
// //   else {
// //     Serial.println("error opening datalog.txt");
// //   }
// }
