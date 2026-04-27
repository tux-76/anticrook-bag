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
  Keypad.h
  ---------------------
  Written by Jonas Blackwood
*/

#ifndef KEYPAD_H
#define KEYPAD_H

#define KEYPAD_R1    5
#define KEYPAD_R2    4
#define KEYPAD_R3    3
#define KEYPAD_R4    2
#define KEYPAD_C1    9
#define KEYPAD_C2    8
#define KEYPAD_C3    7
#define KEYPAD_C4    6

#define ROWS 4
#define COLS 4
int ROW_PINS[ROWS] = {KEYPAD_R1, KEYPAD_R2, KEYPAD_R3, KEYPAD_R4};
int COL_PINS[COLS] = {KEYPAD_C1, KEYPAD_C2, KEYPAD_C3, KEYPAD_C4};

char KEYMAP[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};

class Keypad {
  private:
    bool keyHeld = 0;
  public:
    void setup();
    char getKeyPressed();
};

#endif 