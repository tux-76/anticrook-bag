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