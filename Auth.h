#ifndef AUTH_H
#define AUTH_H

constexpr int KEYCODE_LEN = 4;
const char TRUE_CODE[KEYCODE_LEN] = {'1', '2', '3', '4'};

class Auth {
  public:
  void setup();
  bool checkKeycode(char code[KEYCODE_LEN]);
};

#endif