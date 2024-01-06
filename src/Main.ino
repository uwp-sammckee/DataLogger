#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>

#include "ColorLED.hpp"
#include "Buzzer.hpp"
#include "Memory.h"
#include "State_Machine.h"

#include "Accelerometers/BNO055.h"
#include "Barometers/LPS25HB.h"

#define SERVO_1_PIN 37
#define SERVO_2_PIN 36
#define SERVO_3_PIN 28
#define SERVO_4_PIN 29

#define RECORD_BUTTON 14
bool recording = false;

BNO055 acc;
LPS25HB baro;
Memory memory;
State_Machine stateMachine;

unsigned long loopFreq   = 25; // In Hz
unsigned long loopLenght = 1000 / loopFreq; // In ms
unsigned long loopDelay  = 0;
unsigned long loopStart  = 0;
unsigned long timeStart  = 0;

unsigned long start = 0;
unsigned long end = 0;

specialFloatT data[21];

void setup() {
  Serial.begin(19200);

  delay(500);
  Serial.println("Setup started");

  Wire.begin();
  // ColorLED::begin(); // Commented because the LED is very bright
  Buzzer::begin();
  ColorLED::show_blue();

  pinMode(RECORD_BUTTON, INPUT);

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

  // Start Memory
  if (!memory.begin()) {
    Serial.println("SPI Flash not online");
    while (1);
  }
  Serial.println("SPI Flash detected.");

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
  if (digitalRead(RECORD_BUTTON) == HIGH) {
    recording = !recording;

    delay(500);
    timeStart = millis(); // Reset the time

    // Reset the accelerometer drived data
    acc.reset();

    // Print the header
    Serial.println("Time,\t\tAccX,\tAccY,\tAccZ,\tGyrX,\tGyrY,\tGyrZ,\tMagX,\tMagY,\tMagZ,\tHeading,\tAngleX,\tAngleY,\tAngleZ,\tVelX,\tVelY,\tVelZ,\tTemp,\tPress,\tAlti,\tState");
    memory.write_header();
  }

  if (recording) {
    ColorLED::show_red();

    if ((millis() - loopStart) >= loopLenght) {
      loopStart = millis();

      // Record data
      data[0].value = (millis()-timeStart) / 1000.0f;
      
      acc.get_data(data);
      baro.get_data(data);

      // Update the state machine
      stateMachine.update(data);

      // Write data to memory
      memory.write_data(data);

      // Print data
      Serial.print(data[0].value, 4);
      if (data[0].value < 10) Serial.print(",\t\t");
      else                    Serial.print(",\t");

      for (int i=1; i < 20; i++) {
        Serial.print(data[i].value);
        Serial.print(",\t");
      }
      Serial.println();
    }
  } else {
    if (Serial.available() != 0) {
      char choice = Serial.read(); 

      switch (choice) {
        case 'e': memory.erase_data(); break; // Erase data
        case 'd': memory.dump_to_sd(); break; // Dump data to SD card
        case 'p': memory.print(); break; // Print data

        default: break;
      }
    }
  }
}