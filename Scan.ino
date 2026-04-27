/*
  Anti-Crook Bag: A secure backpack that alarms as a theft detterent.
  Copyright (C) 2026 Jonas Blackwood, Malachi Oldham, Jesse Sivils

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/* 
  Scan.ino
  ---------------------
  Written by Jonas Blackwood
*/

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