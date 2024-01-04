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

BNO055 acc;
LPS25HB baro;

unsigned long loopFreq   = 25; // In Hz
unsigned long loopLenght = 1000 / loopFreq; // In ms
unsigned long loopDelay  = 0;
unsigned long loopStart  = 0;
unsigned long timeStart  = 0;

specialFloatT data[20];

void setup() {
  Serial.begin(19200);

  delay(500);
  Serial.println("Setup started");

  Wire.begin();
  LED.begin();
  buzzer.begin();

  LED.show_red();

  pinMode(PIN_RECORD_DATA, INPUT);

  // Get the sensors online
  // Start Accelerometer
  if (!acc.begin()) {
    Serial.println("Accelerometer not online");
    while (1);
  }
  Serial.println("Accelerometer online");

  // Start Barometer
  if (!baro.begin()) {
    Serial.println("Barometer not online");
    while (1);
  }
  Serial.println("Barometer online");

  // Show good status
  LED.show_green();
  buzzer.start_up_sound();

  Serial.println("Setup finished, everything is good.");
  delay(1000);
}

void loop() {
  LED.show_green();
  recordBTN = digitalRead(PIN_RECORD_DATA);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (recordBTN == HIGH) {
    recording =! recording;

    delay(1000);
    timeStart = millis();

    Serial.println("Time,\tAccX,\tAccY,\tAccZ,\tGyrX,\tGyrY,\tGyrZ,\tMagX,\tMagY,\tMagZ,\tAngleX,\tAngleY,\tAngleZ,\tVelX,\tVelY,\tVelZ,\tTemp,\tPress,\tAlti");
  }

  if (recording) {
    if ((millis() - loopStart) >= loopLenght) {
      loopStart = millis();

      // Record data
      data[0].value = (millis()-timeStart) / 1000.0f;
      acc.get_data(data);
      baro.get_data(data);

      // Print data
      Serial.print(data[0].value, 4);

      for (int i=1; i < 19; i++) {
        Serial.print(",\t");
        Serial.print(data[i].value);
      }
      Serial.println();
    }
  } 
}