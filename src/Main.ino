#include <Wire.h>

#include "ColorLED.h"
#include "Buzzer.h"
#include "Memory.h"

#include "Accelerometers/BNO055.h"
#include "Barometers/LPS25HB.h"

const int PIN_RECORD_DATA = 5;
const int PIN_DUMP_DATA = A2;
const int PIN_ERASE_DATA = A1;

const int PIN_SD_CARD_CS = 7;

bool recordData = false;
int recordDataState = 0; 

bool dumpData = false;
int dumpDataState = 0; 

bool eraseData = false;
int eraseDataState = 0; 

Buzzer buzzer;
ColorLED LED;
Memory memory;

BNO055 acc;
LPS25HB baro;

unsigned long loopLenght = 50;
unsigned long loopDelay  = 0;
unsigned long loopStart  = 0;
unsigned long timeStart  = 0;

specialFloatT data[16];

void setup() {
  Serial.begin(19200);
  Wire.begin();
  
  pinMode(buzzer.get_buzzer_pin(), OUTPUT);

  pinMode(LED.get_red_pin(),  OUTPUT);
  pinMode(LED.get_blue_pin(), OUTPUT);
  pinMode(LED.get_blue_pin(), OUTPUT); 

  pinMode(PIN_RECORD_DATA, INPUT);
  pinMode(PIN_DUMP_DATA,   INPUT);
  pinMode(PIN_ERASE_DATA,  INPUT);

  // Get the sensors online
  acc.begin();
  baro.begin();

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
    data[0].value = (millis()-timeStart) / 1000.0f;

    acc.get_data(data);
    baro.get_data(data);
    
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