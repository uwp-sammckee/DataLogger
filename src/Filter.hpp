// Filter.hpp
#ifndef FILTER_hpp
#define FILTER_hpp

#include <Arduino.h>
#include <ReefwingAHRS.h>

#include "Data.hpp"

class Filter {
	private:
		SensorData sensorData;
		ReefwingAHRS filter;

	public:
		Filter() {
			filter.begin();

			filter.setDOF(DOF::DOF_9);
			filter.setImuType(ImuType::UNKNOWN);
			filter.setFusionAlgorithm(SensorFusion::KALMAN);
			filter.setDeclination(2.01); // Platteville Wisconsin
		}

		void update(Data *data) {
			sensorData.ax = data->accX.value; sensorData.ay = data->accY.value; sensorData.az = data->accZ.value;
			sensorData.gx = data->gyrX.value; sensorData.gy = data->gyrY.value; sensorData.gz = data->gyrZ.value;
			sensorData.mx = data->magX.value; sensorData.my = data->magY.value; sensorData.mz = data->magZ.value;
			sensorData.aTimeStamp = data->accDt.value; sensorData.gTimeStamp = data->gyrDt.value; sensorData.mTimeStamp = data->magDt.value;

			filter.setData(sensorData);
			filter.update();

			data->roll.value = filter.angles.roll;
			data->pitch.value = filter.angles.pitch;
			data->yaw.value = filter.angles.yaw;
		}
};

#endif
