#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>

#include "ColorLED.hpp"
#include "State_Machine.h"
#include "Buzzer.hpp"
#include "Memory.h"
#include "Data.hpp"
#include "Fin_Controller.hpp"
#include "Filter.hpp"

#include "Accelerometers/BNO055.h"
#include "Barometers/LPS25HB.h"
#include "GPS.h"
#include "OtherSensors/PitotTube.hpp"

#define RECORD_BUTTON 14
bool recording = false;

BNO055 acc;
LPS25HB baro;
GPS gps;
PitotTube pitot;

Filter filter;
Memory memory;
State_Machine stateMachine;
Fin_Controller fins;

unsigned long loopFreq   = 60; // In Hz
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

  pinMode(RECORD_BUTTON, INPUT);

  // Get the sensors online
  // Start Accelerometer
  if (!acc.begin()) {
    Serial.println("Accelerometer not online");
    error();
  }
  Serial.println("Accelerometer online");
  acc.get_calibration_offsets();
  acc.write_offsets();
  acc.get_calibration_offsets();

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

  // Start Pitot Tube
  if (!pitot.begin()) {
    Serial.println("Pitot Tube not online");
    error();
  }
  Serial.println("Pitot Tube online");

  Serial.println("Fins setup started");
  fins.begin();
  fins.sweep();

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
      
      // Read sensor data
      acc.get_data(&data);
      baro.get_data(&data);
      gps.get_data(&data);
      // pitot.get_data(&data);

      // Update the fins
      fins.update(&data);

      // Update the state machine
      stateMachine.update(&data);

      // Run the filter
      filter.update(&data);

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
        case 'p': memory.print();      break; // Print data
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
  else 
    ColorLED::show_green();

  recording = !recording;

  delay(500);
  timeStart = millis(); // Reset the time

  // Read the starting alt for the barometer
  baro.get_data(&data);
  data.starting_alt.value = data.alt.value;

  // Print the header
  Serial.println(memory.header);
  memory.write_header();
}