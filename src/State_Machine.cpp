// State_Machine.cpp
#include "State_Machine.h"

State_Machine::State_Machine(int num_stages) {
  state = on_launch_rail;

  this->stagesRemaining = num_stages;
}

State State_Machine::update(specialFloatT* data) {
  switch (state) {
    case on_launch_rail: switch_to_powered_flight(data);    break;
    case powered_flight: switch_to_unpowered_flight(data);  break;
    
    case unpowered_flight:
      switch_to_descent(data);

      // If there is another stage, and we are not in the descent state
      if (stagesRemaining >  1 && state != descent)
        switch_to_powered_flight(data);
      break;

    case descent:
      switch_to_parachute_descent(data);
      
      // Need to catch if there is no parachute deployed and we crash into the ground
      if (state != parachute_descent) switch_to_landed(data);
      
      break;
    case parachute_descent: switch_to_landed(data);             break;
    case landed:            switch_to_powered_flight(data);     break;

    default: break;
  }

  // Update the last altitude
  for (int i = 4; i > 0; i--) {
    lastAltitude[i] = lastAltitude[i - 1];
  }
  lastAltitude[0] = data[19].value;

  // Log the state
  data[20].value = (float)state;

  return state;
}

// The rocket engine has ignited and the rocket is accelerating upwards
void State_Machine::switch_to_powered_flight(specialFloatT* data) {
  // Conditions: vertical acceleration > 5.0 m/s^2
  //    cause on the launch rail SHOULD be 0 m/s^2, and in the air coasting (if mulitstage) SHOULD be -9.8 m/s^2 (or less)
  if (data[0].value > 5.0) {
    state = powered_flight;
  }
}

// The rocket engine has burned out and the rocket is coasting upwards
void State_Machine::switch_to_unpowered_flight(specialFloatT* data) {
  // Conditions: vertical acceleration < -9.8 m/s^2 (±.4 m/s^2)
  if (data[0].value > -9.4 && data[0].value < -10.2) {
    state = unpowered_flight;

    stagesRemaining--;
  }
}

// The rocket has passed the apogee and is descending
void State_Machine::switch_to_descent(specialFloatT* data) {
  // Conditions: altitude is less than the previous altitude (check over a 1 second period)
  
  // Check loop over the last 5 altitudes
  for (int i = 0; i < 5; i++) {
    // Check if the altitude is less than the previous altitude
    if (data[19].value > lastAltitude[i]) {
      return; // If its greater, return
    }
  }
}

// The parachute has been deployed and the rocket is descending
void State_Machine::switch_to_parachute_descent(specialFloatT* data) {
  // Conditions: 
  
}

// The rocket has landed on the ground
void State_Machine::switch_to_landed(specialFloatT* data) {
  // Conditions: acceleration is around 0 m/s^2 (±.5 m/s^2, also check all axis)
  if ((data[0].value > -0.5 && data[0].value < 0.5) ||
      (data[1].value > -0.5 && data[1].value < 0.5) ||
      (data[2].value > -0.5 && data[2].value < 0.5)) {
    state = landed;
  }
}