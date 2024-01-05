// Barometers/LPS25HB.cpp
#include "LPS25HB.h"

LPS25HB::LPS25HB(int address, TwoWire *wire) : Sensor(address, wire) {
    this->temperature = 0.0;
    this->pressure = 0.0;
    this->altitude = 0.0;
}

bool LPS25HB::begin() {
    // Set sample averaging to 16 for temp, and 32 for pressure
    write(RES_CONF_REG, 0x05);

    // Set sample rate to 1 shot 10000000
    write(CTRL_REG1, 0b10000000);

    // Set control register 2 to default
    write(CTRL_REG2, 0x00);

    // Check if WHO_AM_I is correct (should be 0xBD)
    byte who_am_i[1];
    read(WHO_AM_I_REG, who_am_i, 1);

    if (who_am_i[0] != 0xBD) {
        Serial.println("LPS25HB WHO_AM_I incorrect");
        Serial.println(who_am_i[0], HEX);
        return false;
    }

    // Take a sample
    update_sensor();

    return true;
}

void LPS25HB::update_sensor() {
    // Read the current control register 2 value
    byte ctrl_reg2[1];
    read(CTRL_REG2, ctrl_reg2, 1);

    // Toogle the one shot bit
    ctrl_reg2[0] |= 0x01;
    write(CTRL_REG2, ctrl_reg2[0]);

    // Read the pressure
    byte bytes[3];
    read(PRESS_OUT_XL_REG, bytes, 3);
    this->pressure = (float)((int32_t)bytes[0] | (int32_t)(bytes[1] << 8) | (int32_t)(bytes[2] << 16)) / 4096.0;

    // Read the temperature
    byte temp[2];
    read(TEMP_OUT_L_REG, temp, 2);
    this->temperature = 42.5 + ((int16_t)(temp[1] << 8 | temp[0]) / 480.0);

    // Calculate the altitude
    // Equation from https://www.weather.gov/media/epz/wxcalc/pressureAltitude.pdf
    this->altitude = 145366.45 * (1.0 - pow((pressure/1013.25), 0.190284));
}

void LPS25HB::get_data(specialFloatT* data) {
    update_sensor();

    data[16].value = this->temperature;
    data[17].value = this->pressure;
    data[18].value = this->altitude;
}

void LPS25HB::read(int registerAddress, byte* data, int bytes) {
    get_wire()->beginTransmission(get_address());
    
    // If the sensor is a LPS25HB, we need to set the MSB of the register address to 1
    // I fully know why we need this but I saw it in another library and it works
    get_wire()->write(registerAddress | (1 << 7));

    get_wire()->endTransmission(false);

    for (int i=0; i<bytes; i++) {
        get_wire()->requestFrom(get_address(), 1);
        data[i] = get_wire()->read();
    }

    get_wire()->endTransmission(true);
}