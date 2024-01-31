// Servo_Controller.hpp
#ifndef SERVO_CONTROLLER_hpp
#define SERVO_CONTROLLER_hpp

#include <Arduino.h>
#include <Servo.h>

#define SERVO_1_PIN 37
#define SERVO_2_PIN 36
#define SERVO_3_PIN 28
#define SERVO_4_PIN 29

class Servo_Controller {
  private:
    static Servo servo1;
    static Servo servo2;
    static Servo servo3;
    static Servo servo4;

  public:
    static void begin() {
      servo1.attach(SERVO_1_PIN);
      servo2.attach(SERVO_2_PIN);
      servo3.attach(SERVO_3_PIN);
      servo4.attach(SERVO_4_PIN);
    }

    static void setServo1(int angle) {
      servo1.write(angle);
    }

    static void setServo2(int angle) {
      servo2.write(angle);
    }

    static void setServo3(int angle) {
      servo3.write(angle);
    }

    static void setServo4(int angle) {
      servo4.write(angle);
    }
};


#endif