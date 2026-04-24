#ifndef SCAN_H
#define SCAN_H

#define VIN_PIN A0

constexpr int THRESHOLD_ANALOG = 800; // 7V (= V / 10V * 1023) (/10V because voltage divider circuit converts 10V to 5V)

bool checkPluggedMain() {
  return (analogRead(VIN_PIN) > THRESHOLD_ANALOG);
  // return !digitalRead(22);
}

#endif