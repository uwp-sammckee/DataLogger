#include <Arduino.h>
#include <Wire.h>

#include "ColorLED.hpp"
#include "Buzzer.hpp"
// #include "Memory.h"

#include "Accelerometers/BNO055.h"
#include "Barometers/LPS25HB.h"

const int PIN_RECORD_DATA = 14;
// const int PIN_SD_CARD_CS = 7;

bool recording = false;

BNO055 acc;
LPS25HB baro;
// Memory memory;

unsigned long loopFreq   = 25; // In Hz
unsigned long loopLenght = 1000 / loopFreq; // In ms
unsigned long loopDelay  = 0;
unsigned long loopStart  = 0;
unsigned long timeStart  = 0;

unsigned long start = 0;
unsigned long end = 0;

specialFloatT data[20];

void setup() {
  Serial.begin(19200);

  delay(500);
  Serial.println("Setup started");

  Wire.begin();
  // ColorLED::begin(); // Commented because the LED is very bright
  Buzzer::begin();
  ColorLED::show_blue();

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

  delay(500);
  // Setup Finished

  // Show good status
  ColorLED::show_green();
  Buzzer::start_up_sound();

  Serial.println("Setup finished, everything is good.");

  delay(500);
}

void loop() {
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (digitalRead(PIN_RECORD_DATA) == HIGH) {
    recording = !recording;

    delay(500);
    timeStart = millis(); // Reset the time

    // Reset the accelerometer drived data
    acc.reset();

    // Print the header
    Serial.println("Time,\t\tAccX,\tAccY,\tAccZ,\tGyrX,\tGyrY,\tGyrZ,\tMagX,\tMagY,\tMagZ,\tAngleX,\tAngleY,\tAngleZ,\tVelX,\tVelY,\tVelZ,\tTemp,\tPress,\tAlti");
  }

  if (recording) {
    ColorLED::show_red();

    if ((millis() - loopStart) >= loopLenght) {
      loopStart = millis();

      // Record data
      data[0].value = (millis()-timeStart) / 1000.0f;
      
      acc.get_data(data);
      baro.get_data(data);

      // Print data
      Serial.print(data[0].value, 4);
      if (data[0].value < 10) Serial.print(",\t\t");
      else                    Serial.print(",\t");

      for (int i=1; i < 19; i++) {
        Serial.print(data[i].value);
        Serial.print(",\t");
      }
      Serial.println();
    }
  } else {
    ColorLED::show_green();
  }
}