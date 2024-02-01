#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>

#include "ColorLED.hpp"
#include "State_Machine.h"
#include "Buzzer.hpp"
#include "Memory.h"
#include "Data.hpp"
#include "Servo_Controller.hpp"

#include "Accelerometers/BNO055.h"
#include "Barometers/LPS25HB.h"
#include "GPS.h"

#define RECORD_BUTTON 14
bool recording = false;

BNO055 acc;
LPS25HB baro;
GPS gps;
Memory memory;
State_Machine stateMachine;

unsigned long loopFreq   = 30; // In Hz
unsigned long loopLenght = 1000 / loopFreq; // In ms
unsigned long loopDelay  = 0;
unsigned long loopStart  = 0;
unsigned long timeStart  = 0;

unsigned long start = 0;
unsigned long end = 0;

Data data;

void setup() {
  Serial.begin(115200);

  delay(500);
  Serial.println("Setup started");

  Wire.begin();
  
  // ColorLED::begin(); // Commented because the LED is very bright
  Buzzer::begin();
  ColorLED::show_blue();
  // Servo_Controller::begin();

  pinMode(RECORD_BUTTON, INPUT);

  // Get the sensors online
  // Start Accelerometer
  if (!acc.begin()) {
    Serial.println("Accelerometer not online");
    error();
  }
  Serial.println("Accelerometer online");

  // Start Barometer
  if (!baro.begin()) {
    Serial.println("Barometer not online");
    error();
  }
  Serial.println("Barometer online");

  // Start GPS
  if (!gps.begin()) {
    Serial.println("GPS not online");
    error();
  }
  Serial.println("GPS online");

  // Start Memory
  if (!memory.begin()) {
    Serial.println("SPI Flash not online");
    error();
  }
  Serial.println("SPI Flash detected.");

  // Setup Finished

  // Show good status
  ColorLED::show_green();
  Buzzer::start_up_sound();

  Serial.println("Setup finished, everything is good.");
}

void loop() {
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (digitalRead(RECORD_BUTTON) == HIGH) {
    start_recording();
  }

  if (recording) {
    ColorLED::show_red();

    if ((millis() - loopStart) >= loopLenght) {
      loopStart = millis();

      // Record data
      data.time.value = (millis()-timeStart) / 1000.0f;
      
      acc.get_data(&data);
      baro.get_data(&data);
      gps.get_data(&data);

      // Update the state machine
      stateMachine.update(&data);

      // Write data to memory
      memory.write_data(&data);

      // Print data
      Serial.println(data.get_data());
    }
  } else {
    if (Serial.available() != 0) {
      char choice = Serial.read(); 

      switch (choice) {
        case 'e': memory.erase_data(); break; // Erase data
        case 'd': memory.dump_to_sd(); break; // Dump data to SD card
        case 'p': memory.print(&data); break; // Print data
        case 'r': start_recording();   break; // Start recording

        default: break;
      }
    }
  }
}

void error() {
  while (1) {
    Buzzer::error_sound();
    ColorLED::flash_red();
  }
}

void start_recording() {
  if (!recording)
    Buzzer::countdown(1);

  recording = !recording;

  delay(500);
  timeStart = millis(); // Reset the time

  // Reset the accelerometer drived data
  acc.reset();

  // Print the header
  Serial.println(memory.header);
  memory.write_header();
}