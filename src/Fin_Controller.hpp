// Fin_Controller.hpp
#ifndef FIN_CONTROLLER_hpp
#define FIN_CONTROLLER_hpp

#include <Arduino.h>
#include <Servo.h>

#include "Data.hpp"

// The pin's that the servos are connected to
#define SERVO_1_PIN 37
#define SERVO_2_PIN 36
#define SERVO_3_PIN 28
#define SERVO_4_PIN 29

#define SERVO_MAX_ANGLE 40

class Fin_Controller {
  private:
    // Servo objects
    Servo servo1;
    Servo servo2;
    Servo servo3;
    Servo servo4;

    // Servo offsets
    float servo1_offset = 90;
    float servo2_offset = 0;
    float servo3_offset = 0;
    float servo4_offset = 0;

    float angle;

    int pulseWidth;

    // PID variables

    // PID targets
    float target_roll = 0;

    // PID variables
    float error = 0;
    float last_error = 0;

    float output = 0;

    float P = 0;
    float I = 0;
    float D = 0;

    // PID constants
    float const_kp = 4;
    float const_ki = 0.0;
    float const_kd = 0.01;

    const int I_MAX = 2;

    // Time variables
    float last_time = 0;
    float dt;

  public:
    // Allows you to set the angle of the servos
    void setServo1(float angle) {
      angle += servo1_offset;
      angle = constrain(angle, servo1_offset-SERVO_MAX_ANGLE, servo1_offset+SERVO_MAX_ANGLE);

      // Maps and angles to a pulse width for high resolution
      pulseWidth = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
      
      // Writes the pulse width to the servo
      // servo1.writeMicroseconds(pulseWidth);
      servo1.write(angle);
    }

    void setAll(float angle) {
      setServo1(angle);
      // setServo2(angle);
      // setServo3(angle);
      // setServo4(angle);
    }

    // Allows you to set servo offsets
    void setServo1Offset(float offset) { servo1_offset = offset; }
    void setServo2Offset(float offset) { servo2_offset = offset; }
    void setServo3Offset(float offset) { servo3_offset = offset; }
    void setServo4Offset(float offset) { servo4_offset = offset; }

    // Does a test sweep with all of the servos
    void sweep() {
      setServo1(-SERVO_MAX_ANGLE);
      delay(250);

      // Start with the first fin, go from -5 to 5 then back to 0
      for (float i = -SERVO_MAX_ANGLE; i <= SERVO_MAX_ANGLE; i += 0.5) {
        setServo1(i);
        delay(5);
      }
      delay(500);
      setServo1(0);

      delay(1000);

      setServo1(-SERVO_MAX_ANGLE);
      delay(250);

      // Then all of them go from -5 to 5 then back to 0
      for (float i = -SERVO_MAX_ANGLE; i <= SERVO_MAX_ANGLE; i += 0.5) {
        setAll(i);
        delay(5);
      }
      delay(500);
      setAll(0);

    }

    void begin() {
      servo1.attach(SERVO_1_PIN);
      servo2.attach(SERVO_2_PIN);
      servo3.attach(SERVO_3_PIN);
      servo4.attach(SERVO_4_PIN);
    }

    void update(Data *data) {
      // Run the PID, and update the servos
      // We are only going to try and stabilize the roll for now

      // Calculate the time difference
      dt = (millis() - last_time) / 1000.0;
      last_time = millis();

      // Calculate the error
      error = target_roll - data->dir_roll.value;

      // Calculate the P term
      P = error * const_kp;

      // Calculate the I term
      I += error * const_ki * dt;

      // Integral anti-windup
      I = constrain(I, -I_MAX, I_MAX);

      // Calculate the D term
      D = ((error - last_error) / dt) * const_kd;

      // Sum the PID terms
      output = P + I + D;
      
      angle += -output;
      angle = constrain(angle, -SERVO_MAX_ANGLE, SERVO_MAX_ANGLE);

      Serial.print("Error: "); Serial.print(error);
      Serial.print(" P: "); Serial.print(P);
      Serial.print(" I: "); Serial.print(I);
      Serial.print(" D: "); Serial.print(D);
      Serial.print(" Output: "); Serial.print(output);
      Serial.print(" Angle: "); Serial.println(angle);

      // Set the servos
      setAll(angle);

      // Update the last error
      last_error = error;
    }

    Fin_Controller() {
      servo1 = Servo();
      servo2 = Servo();
      servo3 = Servo();
      servo4 = Servo();

      begin();
    }
};


#endif