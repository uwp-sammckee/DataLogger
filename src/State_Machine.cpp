// State_Machine.cpp

#include "State_Machine.h"

State_Machine::State_Machine(){

};

void State_Machine::launch_handler(){

    if (z_accel > 5) {
        state = unpowered_flight;
    }
};

void State_Machine::powered_flight_handler(){
    
    if (z_accel >= 0 && z_accel <= 2) { // checking within a margin of error
        if (z_prev_velocity > z_velocity) {
            state = unpowered_flight;
        }
    }

}

void State_Machine::unpowered_flight_handler() {
    
    if (prev_altitude > altitude) {
        max_apogee = prev_altitude;
        state = descent;
    }

}

void State_Machine::descent_handler() {

    if (z_prev_velocity > z_velocity){ // this may not work depending on how fast the chute deploys
        state = parachute_descent;
    }

}

void State_Machine::parachute_descent_handler() {

    if (z_velocity >= 0 && z_velocity <= 3) {
        state = landed;
    }

}

void State_Machine::landed_handler() {}

void State_Machine::check_state(float accel, float current_altitude){
    float CONSTANT_LOOP_TIME = .050;

    prev_altitude = altitude;
    altitude = current_altitude; 

    // Integrating acceleration to get velocity
    z_prev_velocity = z_velocity;
    z_velocity += accel * CONSTANT_LOOP_TIME;

    z_accel = accel;

    switch (state){
        case on_lanch_rail:
            launch_handler();
            break;
        case powered_flight:
            powered_flight_handler();
            break;
        case unpowered_flight:
            unpowered_flight_handler();
        case descent:
            descent_handler();
        case parachute_descent:
            parachute_descent_handler();
        case landed:
            landed_handler();
        default:
            break;
    }

};