#include "KalmanFilter.h"

KalmanFilter::KalmanFilter() {
    // Initialize matrices and vectors, set initial values
    // Make sure to fill in your specific values for A, B, H, Q, and R matrices

    // Initialize the state vector
    for (int i = 0; i < 9; i++) {
      x[i] = 0.0;
    }

    // Initialize the error covariance matrix
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        P[i][j] = (i == j) ? 1.0 : 0.0; // Set diagonal to 1, rest to 0 (identity matrix)
      }
    }
}

void KalmanFilter::update(float accel[3], float gyro[3], float mag[3], float dt) {
    // Prediction step
    predict(gyro, dt);

    // Measurement update step
    correct(accel, mag);
}

void KalmanFilter::update(Data *data) {
    // Update the Kalman filter with sensor measurements
    float accel[3] = {data->accX.value, data->accY.value, data->accZ.value};
    float gyro[3] = {data->gyrX.value, data->gyrY.value, data->gyrZ.value};
    float mag[3] = {data->magX.value, data->magY.value, data->magZ.value};
    float dt = data->gyr_dt.value;

    update(accel, gyro, mag, dt);

    // Get the estimated state
    float state[9];
    getState(state);

    // Update the data object
    data->kalman_alt.value = state[0];

    data->kalman_x.value = state[1];
    data->kalman_y.value = state[2];
    data->kalman_z.value = state[3];

    data->kalman_roll.value = state[4];
    data->kalman_pitch.value = state[5];
    data->kalman_yaw.value = state[6];
}

void KalmanFilter::getState(float state[9]) {
    // Copy the state vector into the provided array
    for (int i = 0; i < 9; i++) {
        state[i] = x[i];
    }
}

void KalmanFilter::predict(float gyro[3], float dt) {
    // Update the state vector x using the state transition matrix A and control input matrix B
    // x_{k}^- = A * x_{k-1} + B * u_k
    // where u_k is the control input (gyro measurements)
    // This example assumes a simple integration for angular rate
    for (int i = 0; i < 9; i++) {
      x[i] += dt * gyro[i];
    }

    // Update the error covariance matrix P using the state transition matrix A
    // P_{k}^- = A * P_{k-1} * A^T + Q
    // where Q is the process noise covariance
    // This is a simple example; you might need to adjust based on your system
    // In practice, Q should reflect your system's dynamics and uncertainties
}

void KalmanFilter::correct(float accel[3], float mag[3]) {
    // Implement the measurement update step here

    // Compute the Kalman gain K
    // K_k = P_k^- * H^T * (H * P_k^- * H^T + R)^-1
    // where H is the measurement matrix and R is the measurement noise covariance
    // This example assumes direct measurements (H is identity)
    float K[9][9];
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            K[i][j] = P[i][j] / (P[i][i] + R[i][j]); // Simplified for this example
        }
    }

    // Compute the innovation vector z
    // z_k = H * x_k^-
    // where H is the measurement matrix
    float z[9];
    for (int i = 0; i < 9; i++) {
        z[i] = x[i]; // Simplified for this example (H is identity)
    }

    // Update the state vector x
    // x_k = x_k^- + K_k * (z_k - H * x_k^-)
    for (int i = 0; i < 9; i++) {
        x[i] += K[i][i] * (accel[i] - z[i]); // Simplified for this example
    }

    // Update the error covariance matrix P
    // P_k = (I - K_k * H) * P_k^-
    // where I is the identity matrix
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            P[i][j] = (i == j) ? (1 - K[i][i]) * P[i][i] : -K[i][i] * P[i][j]; // Simplified for this example
        }
    }
}