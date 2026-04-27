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
  Scan.h
  ---------------------
  Written by Jonas Blackwood
*/

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