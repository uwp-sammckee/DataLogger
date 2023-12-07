// State_Machine.h
#ifndef STATE_MACHINE_h
#define STATE_MACHINE_h

#include <Arduino.h>


class State_Machine {

    private:
        enum States {
            on_lanch_rail=0,
            powered_flight=1,
            unpowered_flight=2,
            apogee=3,
            descent=4,
            parachute_descent=5,
            landed=6
        };

        float z_prev_velocity = 0.0f;
        float z_velocity = 0.0f;
        float z_accel = 0.0f;

        float prev_altitude = 0.0f;
        float altitude = 0.0f;
        float max_apogee = 0.0f;

        States state;

        void launch_handler();
        void powered_flight_handler();
        void unpowered_flight_handler();
        void descent_handler();
        void parachute_descent_handler();
        void landed_handler();

    public:
        State_Machine();
        void check_state(float accel, float current_altitude);

};

#endif