// Datapoint.h
#ifndef DATAPOINT_h
#define DATAPOINT_h

#include <Arduino.h>

typedef union
{
  float value;
  byte array[4];
}
dataPointT;

const int size = 28;
class Data {
  public:
    dataPointT time;

    dataPointT accX;
    dataPointT accY;
    dataPointT accZ;
    
    dataPointT gyrX;
    dataPointT gyrY;
    dataPointT gyrZ;

    dataPointT magX;
    dataPointT magY;
    dataPointT magZ;
    dataPointT heading;

    dataPointT angleX;
    dataPointT angleY;
    dataPointT angleZ;

    dataPointT roll;
    dataPointT pitch;
    dataPointT yaw;

    dataPointT velX;
    dataPointT velY;
    dataPointT velZ;

    dataPointT temp;
    dataPointT press;
    dataPointT alt;
    
    dataPointT filted_alt;
    dataPointT starting_alt;

    dataPointT state;

    dataPointT gps_lat;
    dataPointT gps_lng;
    dataPointT gps_sat;
    dataPointT gps_alt;
    dataPointT gps_speed;
    dataPointT gps_hdop;

    dataPointT air_speed;

    dataPointT array[size];

    String get_data() {
      String data = "";

      data += String(Data::time.value, 4) + ",";

      // data += String(Data::accX.value, 2) + ",";
      // data += String(Data::accY.value, 2) + ",";
      // data += String(Data::accZ.value, 2) + ",";

      // data += String(Data::gyrX.value, 2) + ",";
      // data += String(Data::gyrY.value, 2) + ",";
      // data += String(Data::gyrZ.value, 2) + ",";

      // data += String(Data::magX.value, 2) + ",";
      // data += String(Data::magY.value, 2) + ",";
      // data += String(Data::magZ.value, 2) + ",";
      // data += String(Data::heading.value, 2) + ",";

      // data += String(Data::angleX.value, 2) + ",";
      // data += String(Data::angleY.value, 2) + ",";
      // data += String(Data::angleZ.value, 2) + ",";

      data += String(Data::roll.value, 2) + ",";
      data += String(Data::pitch.value, 2) + ",";
      data += String(Data::yaw.value, 2) + ",";

      // data += String(Data::velX.value, 2) + ",";
      // data += String(Data::velY.value, 2) + ",";
      // data += String(Data::velZ.value, 2) + ",";

      // data += String(Data::temp.value, 2) + ",";
      // data += String(Data::press.value, 2) + ",";
      // data += String(Data::alt.value, 2) + ",";
      // data += String(Data::filted_alt.value, 2) + ",";

      // data += String(Data::state.value) + ",";

      // data += String(Data::gps_lat.value, 8) + ",";
      // data += String(Data::gps_lng.value, 8) + ",";
      // data += String(Data::gps_sat.value) + ",";
      // data += String(Data::gps_alt.value, 2) + ",";
      // data += String(Data::gps_speed.value, 2) + ",";
      // data += String(Data::gps_hdop.value, 2) + ",";

      return data;
    }

    Data() {
      time.value =       0.f;
      accX.value =       0.f;
      accY.value =       0.f;
      accZ.value =       0.f;
      gyrX.value =       0.f;
      gyrY.value =       0.f;
      gyrZ.value =       0.f;
      magX.value =       0.f;
      magY.value =       0.f;
      magZ.value =       0.f;
      heading.value =    0.f;
      angleX.value =     0.f;
      angleY.value =     0.f;
      angleZ.value =     0.f;
      velX.value =       0.f;
      velY.value =       0.f;
      velZ.value =       0.f;
      temp.value =       0.f;
      press.value =      0.f;
      alt.value =        0.f;
      state.value =      0.f;
      gps_lat.value =    0.f;
      gps_lng.value =    0.f;
      gps_sat.value =    0.f;
      gps_alt.value =    0.f;
      gps_speed.value =  0.f;
      gps_hdop.value =   0.f;
    }
};

#endif