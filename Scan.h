#ifndef SCAN_H
#define SCAN_H

#define VIN_PIN A11

constexpr int THRESHOLD_ANALOG = 716; // 7V (= V / 10V * 1023) (/10V because voltage divider circuit converts 10V to 5V)

bool checkPluggedMain() {
  // return 0;
  return (analogRead(VIN_PIN) > THRESHOLD_ANALOG);
}

#endif