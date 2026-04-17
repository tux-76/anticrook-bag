#include "Keypad.h"

void Keypad::setup() {
  Serial.println("Setting up keypad!");
  pinMode(KEYPAD_R1, INPUT);
  pinMode(KEYPAD_R2, INPUT);
  pinMode(KEYPAD_R3, INPUT);
  pinMode(KEYPAD_R4, INPUT);
  pinMode(KEYPAD_C1, OUTPUT);
  pinMode(KEYPAD_C2, OUTPUT);
  pinMode(KEYPAD_C3, OUTPUT);
  pinMode(KEYPAD_C4, OUTPUT);  
}

char Keypad::getKeyPressed() {
  for (int c = 0; c < COLS; c++) {
    int cPin = COL_PINS[c];
    digitalWrite(cPin, HIGH);
    for (int r = 0; r < ROWS; r++) {
      bool pressed = digitalRead(ROW_PINS[r]);
      if (pressed) {
        if (!keyHeld) {
          keyHeld = 1;
          return KEYMAP[r][c];
        } else {
          return 0;
        }
      }
    }
    digitalWrite(cPin, LOW);
  }
  keyHeld = 0;
  return 0;
}