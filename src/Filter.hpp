// Filter.hpp
#ifndef FILTER_hpp
#define FILTER_hpp

#include <Arduino.h>
#include <MadgwickAHRS.h>

#include "Data.hpp"

class Filter {
	private:
		Madgwick filter;

	public:
		Filter() {
			filter.begin(0.033);
		}

		void update(Data *data) {
			filter.update(
				data->gyrX.value, data->gyrY.value, data->gyrZ.value,
				data->accX.value, data->accY.value, data->accZ.value,
				data->magX.value, data->magY.value, data->magZ.value
			);

			data->roll.value	= filter.getRoll();
			data->pitch.value	= filter.getPitch();
			data->yaw.value		= filter.getYaw();
		}
};

#endif
