// Pyro.hpp
#ifndef PYRO_h
#define PYRO_h

#include <Arduino.h>

#define CHANNEL_1_TRIGER_PIN 38
#define CHANNEL_2_TRIGER_PIN 40
#define CHANNEL_3_TRIGER_PIN 27
#define CHANNEL_4_TRIGER_PIN 25

#define CHANNEL_1_STATUS_PIN 39
#define CHANNEL_2_STATUS_PIN 41
#define CHANNEL_3_STATUS_PIN 26
#define CHANNEL_4_STATUS_PIN 24

// Used to control the pyro channels (IC: VN5E160S-E)
class PyroChannel {
  private:
    int trigerPin;
    int statusPin;

  public:
    PyroChannel(int trigerPin, int statusPin) {
      this->trigerPin = trigerPin;
      this->statusPin = statusPin;
    }

    void begin() {
      pinMode(this->trigerPin, OUTPUT);
      pinMode(this->statusPin, INPUT);
    }
    
    bool conntinuity() {
      // NEED PULL UP RESISTOR ON THE OUTPUT SIDE
      // Datasheet page 24 (10k ohm should be fine)
      return digitalRead(this->statusPin);
    }

    bool armed() {
      // TODO: Implement
      // Find a way to see if the chips have power.
      // Checks to see if PYRO_SWITCH is switched


    }

    void fire() {
      digitalWrite(this->trigerPin, HIGH);
    }
};

#endif