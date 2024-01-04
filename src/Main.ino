#include <Arduino.h>
#include <Wire.h>

#include "ColorLED.h"
#include "Buzzer.h"
// #include "Memory.h"

#include "Accelerometers/BNO055.h"
#include "Barometers/LPS25HB.h"

const int PIN_RECORD_DATA = 14;

// const int PIN_SD_CARD_CS = 7;

bool recording = false;
int recordBTN = 0;

Buzzer buzzer;
ColorLED LED;
// Memory memory;

// BNO055 acc;
LPS25HB baro;

unsigned long loopFreq   = 25; // In Hz
unsigned long loopLenght = 1000 / loopFreq; // In ms
unsigned long loopDelay  = 0;
unsigned long loopStart  = 0;
unsigned long timeStart  = 0;

specialFloatT data[20];

void setup() {
  Serial.begin(19200);

  delay(1000);
  Serial.println("Setup started");

  Wire.begin();
  
  pinMode(LED.get_red_pin(),  OUTPUT);
  pinMode(LED.get_blue_pin(), OUTPUT);
  pinMode(LED.get_blue_pin(), OUTPUT); 

  pinMode(buzzer.get_buzzer_pin(), OUTPUT);
  pinMode(PIN_RECORD_DATA, INPUT);

  // Get the sensors online
  // if (!acc.begin()) {
  //   Serial.println("Accelerometer not online");
  //   while (1);
  // }
  // Serial.println("Accelerometer online");

  if (!baro.begin()) {
    Serial.println("Barometer not online");
    while (1);
  }
  Serial.println("Barometer online");

  // buzzer.start_up_sound();
  Serial.println("Setup finished, everything is good.");
  delay(1000);

  Serial.println("Time,\t\tAccX,\tAccY,\tAccZ,\tGyrX,\tGyrY,\tGyrZ,\tMagX,\tMagY,\tMagZ,\tAngleX,\tAngleY,\tAngleZ,\tVelX,\tVelY,\tVelZ,\tTemp,\tPress,\tAlti");
}

void loop() {
  
  LED.show_blue();
  recordBTN = digitalRead(PIN_RECORD_DATA);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (recordBTN == HIGH) {
    recording =! recording;

    delay(1000);
    timeStart = millis();
  }

  if (recording) {
    if ((millis() - loopStart) >= loopLenght) {
      loopStart = millis();

      // Record data
      data[0].value = (millis()-timeStart) / 1000.0f;
      // acc.get_data(data);
      baro.get_data(data);

      Serial.print(data[0].value, 4);
      if (data[0].value < 10) Serial.print(",\t\t");
      else                    Serial.print(",\t");

      for (int i=1; i < 19; i++) {
        Serial.print(data[i].value);
        Serial.print(",\t");
      }
      Serial.println();
    }
  } 
}