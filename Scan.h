#ifndef SCAN_H
#define SCAN_H

#include "Accelerometer.h"

#define VIN_PIN A0

// Errors
#define ERROR_ACCEL_NOT_CONNECTED 10

constexpr int VOLTAGE_THRESHOLD = 800;
constexpr float MOVEMENT_THRESHOLD = 2.0;

class Scan {
  private:
    Accelerometer accel;
    bool accelOperational = 1;

  public:
    int setup();
    void tick();

    bool checkPluggedMain();
    bool checkAccelMovement();
};

#endif