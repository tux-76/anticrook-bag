#include "Auth.h"

void Auth::setup() {
  
}

bool Auth::checkKeycode(char* keycode) {
  bool authenticated = 1;
  for (int i = 0; i < KEYCODE_LEN; i++) {
    if (keycode[i] != TRUE_CODE[i]) authenticated = 0;
  }
  return authenticated;
}