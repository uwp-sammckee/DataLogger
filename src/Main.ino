#include "Wire.h"
#include <MPU6050_light.h>

#include "ColorLED.h"
#include "Buzzer.h"
#include "Memory.h"
#include "Accelerometer.h"
#include "Barometer.h"
#include "State_Machine.h"

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
Accelerometer accelerometer(mpu);
Barometer baro;
Memory memory;

State_Machine state_machine(accelerometer);

const unsigned long LOOP_LENGTH = 50;
unsigned long loopDelay  = 0;
unsigned long loopStart  = 0;
unsigned long timeStart  = 0;

specialFloatT data[16];

void setup() {
  Serial.begin(19200);
  
  Wire.begin();
  byte status = mpu.begin();
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
    
    baro.get_alt_pres_temp(data);
    // Serial.print(data[3].value);
    // Serial.print("/");
    // Serial.print(data[4].value);
    // Serial.print("/");
    // Serial.println(data[5].value);

    data[12].value = (millis()-timeStart) / 1000.0f;

    data[13].value = (float)state_machine.check_state(data[6].value, data[3].value);

    Serial.println();
    memory.write_data(data);
  }

  if (dumpData) {
    buzzer.start_sound();
    LED.show_red();
    memory.print_data_to_serial();
    buzzer.end_sound();
    dumpData = false;
  }
  
  loopDelay = LOOP_LENGTH - (millis() - loopStart);
  if (loopDelay > LOOP_LENGTH) loopDelay = 0;

  delay(loopDelay);
}