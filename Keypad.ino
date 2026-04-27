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
  Keypad.ino
  ---------------------
  Written by Jonas Blackwood
*/

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