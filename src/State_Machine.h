// State_Machine.h
#ifndef STATE_MACHINE_h
#define STATE_MACHINE_h

#include <Arduino.h>
#include "Accelerometer.h"


class State_Machine {

    private:
        enum States {
            on_lanch_rail,
            powered_flight,
            unpowered_flight,
            descent,
            parachute_descent,
            landed
        };

        float z_prev_velocity = 0.0f;
        float z_velocity = 0.0f;
        float z_accel = 0.0f;

        float prev_altitude = 0.0f;
        float altitude = 0.0f;
        float max_apogee = 0.0f;

        int stateInt = 0;

        States state = States::on_lanch_rail;

        void launch_handler();
        void powered_flight_handler();
        void unpowered_flight_handler();
        void descent_handler();
        void parachute_descent_handler();
        void landed_handler();

        Accelerometer &accelerometer; 

    public:
        State_Machine(Accelerometer &accelerometer);
        int get_state(){return (int)state;}
        int check_state(float accel, float current_altitude);

};

#endif