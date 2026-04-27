#include "Accelerometer.h"

// === SETUP ===
bool Accelerometer::setup() {
  Serial.println("Adafruit MPU6050 setup!");

  // Try to initialize
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    return 1;
  }
  Serial.println("MPU6050 Found!");
  //We're not using the accelerometer, but when I removed this code, it wouldn't compile correctly
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  }
  //Gyro settings. I'm keeping the range to 500 deg because it works well enough here; can go up to 2000 in increments of 500
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  }
  //Filter bandwidth settings. Determines how frequently the gyro itself takes reading; currently set to second lowest
  mpu.setFilterBandwidth(MPU6050_BAND_10_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  }
  //Establishing Variables so alarm doesn't trip immediately
  sensors_event_t a, g, temp;  mpu.getEvent(&a, &g, &temp);
  pitCurX = g.gyro.x;  pitCurY = g.gyro.y;  pitCurZ = g.gyro.z;
  pitPrevX = pitCurX;  pitPrevY = pitCurY;  pitPrevZ = pitCurZ;

  return 0;
}

// === TICK ===
void Accelerometer::tick() {
  Counter++;

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  //******************A bunch of serial monitor stuff, can be removed for space
  // Serial.print("Rotation X: ");
  // Serial.print(g.gyro.x);
  // Serial.print(", Y: ");
  // Serial.print(g.gyro.y);
  // Serial.print(", Z: ");
  // Serial.print(g.gyro.z);
  // Serial.println(" rad/s");


  // Serial.print("Temperature: ");
  // Serial.print(temp.temperature);
  // Serial.println(" degC");
  //*******************My serial monitor stuff, can also be removed for space but is nice for diagnostic purposes
  // Serial.println("");
  // Serial.println(Counter);
  // Serial.println("");
  // Serial.print(pitCurX); Serial.print("   "); Serial.print(pitCurY); Serial.print("   "); Serial.print(pitCurZ); Serial.print("   ");
  // Serial.println("");
  // Serial.print(pitPrevX); Serial.print("   "); Serial.print(pitPrevY); Serial.print("   "); Serial.print(pitPrevZ); Serial.print("   ");
  // Serial.println("");
//Takes measurement 1
  if(Counter == ACCELEROMETER_WAIT_TIME) {
    pitPrevX = g.gyro.x;
    pitPrevY = g.gyro.y;
    pitPrevZ = g.gyro.z;
  }
//Takes measurement 2 and resets counter
  if(Counter >= ACCELEROMETER_WAIT_TIME*2) {
    Counter = 0;
    pitCurX = g.gyro.x;
    pitCurY = g.gyro.y;
    pitCurZ = g.gyro.z;
  }
}

float Accelerometer::calcMovement() {
  //Actually evaluating the gyro; abs function takes absolute value
  float EvalX = abs(pitPrevX - pitCurX);
  float EvalY = abs(pitPrevY - pitCurY);
  float EvalZ = abs(pitPrevZ - pitCurZ);
  //alarm code
  float movement = EvalX + EvalY + EvalZ;
  return movement;
}