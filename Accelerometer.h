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
  Accelerometer.h
  ---------------------
  Written by Malachi Oldham,
  Adapted by Jonas Blackwood
*/

#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

constexpr int ACCELEROMETER_WAIT_TIME = 10;

class Accelerometer {
  private:
    Adafruit_MPU6050 mpu;
    //(some of) Our Variables //NOTE: prev/current are holdovers from a previous plan, they don't actually read off each other, they just take two seperate readings at different times
    float pitCurX = 0;
    float pitCurY = 0;
    float pitCurZ = 0;
    float pitPrevX = 0;
    float pitPrevY = 0;
    float pitPrevZ = 0;
    int Counter = 0;
  public:
    bool setup();
    void tick();
    float calcMovement();
};

#endif