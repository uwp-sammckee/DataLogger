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

const int size = 29;
class Data {
  public:
    dataPointT time;

    dataPointT accX;
    dataPointT accY;
    dataPointT accZ;
    dataPointT accDt;
    
    dataPointT gyrX;
    dataPointT gyrY;
    dataPointT gyrZ;
    dataPointT gyrDt;

    dataPointT magX;
    dataPointT magY;
    dataPointT magZ;
    dataPointT magDt;
    dataPointT heading;

    dataPointT dir_roll;

    dataPointT roll;
    dataPointT pitch;
    dataPointT yaw;

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

    dataPointT pid_p;
    dataPointT pid_i;
    dataPointT pid_d;
    
    dataPointT pid_o;
    dataPointT pid_e;

    dataPointT roll_angle;

    dataPointT array[size];

    String get_data() {
      String data = "";

      data += String(Data::time.value, 4) + ",";

      data += String(Data::accX.value, 2) + ",";
      data += String(Data::accY.value, 2) + ",";
      data += String(Data::accZ.value, 2) + ",";

      data += String(Data::gyrX.value, 2) + ",";
      data += String(Data::gyrY.value, 2) + ",";
      data += String(Data::gyrZ.value, 2) + ",";

      data += String(Data::magX.value, 2) + ",";
      data += String(Data::magY.value, 2) + ",";
      data += String(Data::magZ.value, 2) + ",";
      data += String(Data::heading.value, 2) + ",";

      // data += String(Data::roll.value, 2) + ",";
      // data += String(Data::pitch.value, 2) + ",";
      // data += String(Data::yaw.value, 2) + ",";

      data += String(Data::dir_roll.value, 2) + ",";

      data += String(Data::temp.value, 2) + ",";
      data += String(Data::press.value, 2) + ",";
      data += String(Data::alt.value, 2) + ",";
      data += String(Data::filted_alt.value, 2) + ",";

      data += String(Data::state.value) + ",";

      data += String(Data::gps_lat.value, 8) + ",";
      data += String(Data::gps_lng.value, 8) + ",";
      data += String(Data::gps_sat.value) + ",";
      data += String(Data::gps_alt.value, 2) + ",";
      data += String(Data::gps_speed.value, 2) + ",";
      data += String(Data::gps_hdop.value, 2) + ",";

      data += String(Data::air_speed.value, 2) + ",";

      data += String(Data::pid_p.value, 2) + ",";
      data += String(Data::pid_i.value, 2) + ",";
      data += String(Data::pid_d.value, 2) + ",";

      data += String(Data::pid_o.value, 2) + ",";
      data += String(Data::pid_e.value, 2) + ",";

      data += String(Data::roll_angle.value, 2) + ",";

      return data;
    }

    Data() { }
};

#endif