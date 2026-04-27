#include "Scan.h"


int Scan::setup() {
  if (accel.setup()) {
    accelOperational = 0;
    return ERROR_ACCEL_NOT_CONNECTED;
  }
  return 0;
}

void Scan::tick() {
  if (accelOperational) accel.tick();
}

bool Scan::checkPluggedMain() {
  // Serial.println(analogRead(VIN_PIN));
  return (analogRead(VIN_PIN) > VOLTAGE_THRESHOLD);
  // return !digitalRead(22);
}

bool Scan::checkAccelMovement() {
  float movement = accel.calcMovement();
  Serial.println(movement);
  return (movement > MOVEMENT_THRESHOLD);
}