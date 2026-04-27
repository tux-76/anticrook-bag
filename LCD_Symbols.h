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
  LCD_Symbols.h
  ---------------------
  Written by Jonas Blackwood
*/

#ifndef LCD_SYMBOLS_H
#define LCD_SYMBOLS_H

// === SYMBOLS ===
byte SYMBOL_LOCKED_IMG[8] = {
  0b00000,
  0b01110,
  0b10001,
  0b10001,
  0b11111,
  0b11011,
  0b11011,
  0b11111,
};

byte SYMBOL_UNLOCKED_IMG[8] = {
  0b01110,
  0b10000,
  0b10000,
  0b10000,
  0b11111,
  0b11011,
  0b11011,
  0b11111,
};

byte SYMBOL_WARNING_IMG[8] = {
  B00100,
  B01110,
  B01010,
  B01010,
  B11011,
  B11111,
  B11011,
  B11111
};

byte SYMBOL_PLUG_IMG[8] = { B00000, B01010, B01010, B11111, B01110, B01110, B00100, B00100 };
byte SYMBOL_BATT_IMG[8] = { B00100, B11111, B10001, B11111, B11111, B11111, B11111, B11111 };

// Indexes
constexpr byte SYMBOL_LOCKED = 0;
constexpr byte SYMBOL_UNLOCKED = 1;
constexpr byte SYMBOL_WARNING = 2;
constexpr byte SYMBOL_PLUG = 3;
constexpr byte SYMBOL_BATT = 4;

#endif
