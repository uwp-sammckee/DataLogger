// State_Machine.cpp

#include "State_Machine.h"

State_Machine::State_Machine(Accelerometer &accelerometer){
    this->accelerometer = accelerometer;
};

void State_Machine::launch_handler(){

    if (z_accel > 1) {
        state = powered_flight;
    }
}

void State_Machine::powered_flight_handler(){
    
    if (z_accel < 0) { // checking within a margin of error
        state = unpowered_flight;
    }

}

void State_Machine::unpowered_flight_handler() {
    
    if (prev_altitude > altitude) {
        max_apogee = prev_altitude;
        state = descent;
    }

}

void State_Machine::descent_handler() {

    if (z_prev_velocity > accelerometer.get_x_velocity()){ // this may not work depending on how fast the chute deploys
        state = parachute_descent;
    }

}

void State_Machine::parachute_descent_handler() {

    

    if (accelerometer.get_x_velocity() <= 0 && accelerometer.get_x_velocity() >= -1) {
        state = landed;
    }

}

void State_Machine::landed_handler() {}

int State_Machine::check_state(float accel, float current_altitude){
    prev_altitude = altitude;
    altitude = current_altitude; 

    // Integrating acceleration to get velocity
    z_prev_velocity = accelerometer.get_x_velocity();

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
            break;
        case descent:
            descent_handler();
            break;
        case parachute_descent:
            parachute_descent_handler();
            break;
        case landed:
            landed_handler();
            break;
        default:
            break;
    }

    return state;

};