#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

#include "Data.hpp"

class KalmanFilter {
private:
  // State vector
  float x[9]; // Altitude, Relative Position (X, Y, Z), Relative Rotation (Roll, Pitch, Yaw)

  // State transition matrix
  float A[9][9]; // Define your state transition matrix

  // Control input matrix
  float B[9][6]; // Define your control input matrix

  // Measurement matrix
  float H[9][9]; // Define your measurement matrix

  // Process noise covariance
  float Q[9][9]; // Define your process noise covariance

  // Measurement noise covariance
  float R[9][9]; // Define your measurement noise covariance

  // Error covariance matrix
  float P[9][9];

  // Prediction step
  void predict(float gyro[3], float dt);

  // Measurement update step
  void correct(float accel[3], float mag[3]);

public:
  KalmanFilter();

  // Update the Kalman filter with sensor measurements
  void update(float accel[3], float gyro[3], float mag[3], float dt);

  void update(Data *data);

  // Get the estimated state
  void getState(float state[9]);
};

#endif