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

    dataPointT velX;
    dataPointT velY;
    dataPointT velZ;

    dataPointT temp;
    dataPointT press;
    dataPointT alt;

    dataPointT state;

    dataPointT gps_lat;
    dataPointT gps_lng;
    dataPointT gps_sat;
    dataPointT gps_alt;
    dataPointT gps_speed;
    dataPointT gps_hdop;

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

      data += String(Data::angleX.value, 2) + ",";
      data += String(Data::angleY.value, 2) + ",";
      data += String(Data::angleZ.value, 2) + ",";

      data += String(Data::velX.value, 2) + ",";
      data += String(Data::velY.value, 2) + ",";
      data += String(Data::velZ.value, 2) + ",";

      data += String(Data::temp.value, 2) + ",";
      data += String(Data::press.value, 2) + ",";
      data += String(Data::alt.value, 2) + ",";

      data += String(Data::state.value) + ",";

      data += String(Data::gps_lat.value, 8) + ",";
      data += String(Data::gps_lng.value, 8) + ",";
      data += String(Data::gps_sat.value) + ",";
      data += String(Data::gps_alt.value, 2) + ",";
      data += String(Data::gps_speed.value, 2) + ",";
      data += String(Data::gps_hdop.value, 2) + ",";

      return data;
    }

    byte* getDatapoints() {
      byte array[size * 4] = { 
        time.array[0], time.array[1], time.array[2], time.array[3],

        accX.array[0], accX.array[1], accX.array[2], accX.array[3],
        accY.array[0], accY.array[1], accY.array[2], accY.array[3],
        accZ.array[0], accZ.array[1], accZ.array[2], accZ.array[3],

        gyrX.array[0], gyrX.array[1], gyrX.array[2], gyrX.array[3],
        gyrY.array[0], gyrY.array[1], gyrY.array[2], gyrY.array[3],
        gyrZ.array[0], gyrZ.array[1], gyrZ.array[2], gyrZ.array[3],

        magX.array[0], magX.array[1], magX.array[2], magX.array[3],
        magY.array[0], magY.array[1], magY.array[2], magY.array[3],
        magZ.array[0], magZ.array[1], magZ.array[2], magZ.array[3],
        heading.array[0], heading.array[1], heading.array[2], heading.array[3],

        angleX.array[0], angleX.array[1], angleX.array[2], angleX.array[3],
        angleY.array[0], angleY.array[1], angleY.array[2], angleY.array[3],
        angleZ.array[0], angleZ.array[1], angleZ.array[2], angleZ.array[3],

        velX.array[0], velX.array[1], velX.array[2], velX.array[3],
        velY.array[0], velY.array[1], velY.array[2], velY.array[3],
        velZ.array[0], velZ.array[1], velZ.array[2], velZ.array[3],

        temp.array[0], temp.array[1], temp.array[2], temp.array[3],
        press.array[0], press.array[1], press.array[2], press.array[3],
        alt.array[0], alt.array[1], alt.array[2], alt.array[3],

        state.array[0], state.array[1], state.array[2], state.array[3],

        gps_lat.array[0], gps_lat.array[1], gps_lat.array[2], gps_lat.array[3],
        gps_lng.array[0], gps_lng.array[1], gps_lng.array[2], gps_lng.array[3],
        gps_sat.array[0], gps_sat.array[1], gps_sat.array[2], gps_sat.array[3],
        gps_alt.array[0], gps_alt.array[1], gps_alt.array[2], gps_alt.array[3],
        gps_speed.array[0], gps_speed.array[1], gps_speed.array[2], gps_speed.array[3],
      };

      return array;
    }

    void read(byte* array) {
      time.array[0] = array[0]; time.array[1] = array[1]; time.array[2] = array[2]; time.array[3] = array[3];

      accX.array[0] = array[4]; accX.array[1] = array[5]; accX.array[2] = array[6]; accX.array[3] = array[7];
      accY.array[0] = array[8]; accY.array[1] = array[9]; accY.array[2] = array[10]; accY.array[3] = array[11];
      accZ.array[0] = array[12]; accZ.array[1] = array[13]; accZ.array[2] = array[14]; accZ.array[3] = array[15];

      gyrX.array[0] = array[16]; gyrX.array[1] = array[17]; gyrX.array[2] = array[18]; gyrX.array[3] = array[19];
      gyrY.array[0] = array[20]; gyrY.array[1] = array[21]; gyrY.array[2] = array[22]; gyrY.array[3] = array[23];
      gyrZ.array[0] = array[24]; gyrZ.array[1] = array[25]; gyrZ.array[2] = array[26]; gyrZ.array[3] = array[27];

      magX.array[0] = array[28]; magX.array[1] = array[29]; magX.array[2] = array[30]; magX.array[3] = array[31];
      magY.array[0] = array[32]; magY.array[1] = array[33]; magY.array[2] = array[34]; magY.array[3] = array[35];
      magZ.array[0] = array[36]; magZ.array[1] = array[37]; magZ.array[2] = array[38]; magZ.array[3] = array[39];
      heading.array[0] = array[40]; heading.array[1] = array[41]; heading.array[2] = array[42]; heading.array[3] = array[43];

      angleX.array[0] = array[44]; angleX.array[1] = array[45]; angleX.array[2] = array[46]; angleX.array[3] = array[47];
      angleY.array[0] = array[48]; angleY.array[1] = array[49]; angleY.array[2] = array[50]; angleY.array[3] = array[51];
      angleZ.array[0] = array[52]; angleZ.array[1] = array[53]; angleZ.array[2] = array[54]; angleZ.array[3] = array[55];

      velX.array[0] = array[56]; velX.array[1] = array[57]; velX.array[2] = array[58]; velX.array[3] = array[59];
      velY.array[0] = array[60]; velY.array[1] = array[61]; velY.array[2] = array[62]; velY.array[3] = array[63];
      velZ.array[0] = array[64]; velZ.array[1] = array[65]; velZ.array[2] = array[66]; velZ.array[3] = array[67];

      temp.array[0] = array[68]; temp.array[1] = array[69]; temp.array[2] = array[70]; temp.array[3] = array[71];
      press.array[0] = array[72]; press.array[1] = array[73]; press.array[2] = array[74]; press.array[3] = array[75];
      alt.array[0] = array[76]; alt.array[1] = array[77]; alt.array[2] = array[78]; alt.array[3] = array[79];

      state.array[0] = array[80]; state.array[1] = array[81]; state.array[2] = array[82]; state.array[3] = array[83];

      gps_lat.array[0] = array[84]; gps_lat.array[1] = array[85]; gps_lat.array[2] = array[86]; gps_lat.array[3] = array[87];
      gps_lng.array[0] = array[88]; gps_lng.array[1] = array[89]; gps_lng.array[2] = array[90]; gps_lng.array[3] = array[91];
      gps_sat.array[0] = array[92]; gps_sat.array[1] = array[93]; gps_sat.array[2] = array[94]; gps_sat.array[3] = array[95];
      gps_alt.array[0] = array[96]; gps_alt.array[1] = array[97]; gps_alt.array[2] = array[98]; gps_alt.array[3] = array[99];
      gps_speed.array[0] = array[100]; gps_speed.array[1] = array[101]; gps_speed.array[2] = array[102]; gps_speed.array[3] = array[103];
      gps_hdop.array[0] = array[104]; gps_hdop.array[1] = array[105]; gps_hdop.array[2] = array[106]; gps_hdop.array[3] = array[107];
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