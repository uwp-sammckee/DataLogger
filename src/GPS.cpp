#include "GPS.h"

GPS::GPS(HardwareSerial* serial, int baud) : Sensor(0x00) { // No address for GPS cause it's serial
  this->serial = serial;
  this->baud = baud;
}

bool GPS::begin() {
  this->serial->begin(9600);

  return true;
}

void GPS::update_sensor() {
  // Read the Serial String and process it
  // THIS COULD BE A BOTTLNECK
  while (serial->available() > 0) {
    gps.encode(serial->read());
  }
}

void GPS::get_data(specialFloatT* data) {
  update_sensor();

  data[21].value = gps.location.lat();
  data[22].value = gps.location.lng();
  data[23].value = gps.satellites.value();
  data[24].value = gps.altitude.meters();
  data[25].value = gps.speed.kmph();
  data[26].value = gps.hdop.hdop();

  // Read the GPS data
  // Decode the NMEA Data (https://docs.arduino.cc/learn/communication/gps-nmea-data-101)
  // Most likely the module we are going to fly (https://cdn-shop.adafruit.com/product-files/746/CD+PA1616S+Datasheet.v03.pdf)
}

