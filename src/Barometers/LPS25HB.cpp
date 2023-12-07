// Barometers/LPS25HB.cpp
#include "LPS25HB.h"

LPS25HB::LPS25HB(int address, TwoWire *wire) : Sensor(address, wire) {
    this->temperature = 0.0;
    this->pressure = 0.0;
    this->altitude = 0.0;
}

void LPS25HB::begin() {
    // Set sample averaging to 16 for temp, and 32 for pressure
    write(RES_CONF_REG, 0x05);

    // Set sample rate to 1 shot
    write(CTRL_REG1, 0x00);

    // Set control register 2 to default
    write(CTRL_REG2, 0x00);

    // Take a sample
    update_sensor();
}

void LPS25HB::update_sensor() {
    // Read the current control register 2 value
    byte* ctrl_reg2 = 0x00;
    read(CTRL_REG2, ctrl_reg2);

    // Toogle the one shot bit
    *ctrl_reg2 |= 0x01;
    write(CTRL_REG2, *ctrl_reg2);

    // Read the pressure
    byte bytes[3];
    read(PRESS_OUT_XL_REG, bytes, 3);

    this->pressure = ((bytes[2] << 16) | (bytes[1] << 8) | bytes[0]) / 4096.0;

    // Read the temperature
    this->temperature = read_float(TEMP_OUT_L_REG, 1.0);

    // Calculate the altitude
    // Equation from https://www.weather.gov/media/epz/wxcalc/pressureAltitude.pdf
    this->altitude = 145366.45 * (1.0 - pow((pressure/1013.25), 0.190284));
}

void LPS25HB::get_data(specialFloatT* data) {
    data[16].value = this->temperature;
    data[17].value = this->pressure;
    data[18].value = this->altitude;
}
