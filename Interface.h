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
  Interface.h
  ---------------------
  Written by Jonas Blackwood
*/

#ifndef INTERFACE_H
#define INTERFACE_H

#include "LCD_Symbols.h"
#include "Keypad.h"
#include "Scan.h" // For error codes
#include "Auth.h"
#include "Sound.h"

// === LCD SCREEN ===
constexpr int LCD_RS=52, LCD_EN=53, LCD_D4=48, LCD_D5=49, LCD_D6=50, LCD_D7=51;
// constexpr int LCD_RS=52, LCD_EN=53, LCD_D4=48, LCD_D5=49, LCD_D6=50, LCD_D7=51;
constexpr int BUTTON_PIN = 22;

// === INTERFACE ===
class Interface {
  private:
    LiquidCrystal lcd;

    Keypad keypad;
    Sound sound;

    bool authenticated = 0;

    unsigned int notifyTime = 0;

    char keycode[KEYCODE_LEN];
    int keycodeLen = 0;
    void checkKeyPressed();

    void tickNotify();

    bool _armedPlug = 0;    // Varialbles for if data needs to be rewritten
    bool _armedAccel = 0;
    bool _armedPhoto = 0;
    bool _plugged = 0;

    int backpackError = 0;

  public:
    Interface() : lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7) {
      pinMode(BUTTON_PIN, INPUT);
    };
    void registerError(int code);

    bool checkKeycodeIn();
    char* getKeycode();
    void clearKeycode();
    bool keycodesAreEqual(char* code1, char* code2);

    void notifyUnauth();
    void notifyAccelError(); // Depricated
    void notifyAccel();
    void notifyPackUnsecure();
    void notifyKeycodeResetError();
    void notifyKeycodeResetSuccess();
    void endNotify();

    void displayKeycodePrevCode();
    void displayKeycodeNewCode();

    void displayArmedPlug();
    void displayArmedAccel();
    void displayDisarmed();
    void displayArmStatus(bool armedPlug, bool armedAccel) {
      if (armedPlug) displayArmedPlug();
      else if (armedAccel) displayArmedAccel();
      else displayDisarmed();
    }
    void displayPlugged();
    void displayUnplugged();
    void displayPlugStatus(bool plugged) {
      if (plugged) displayPlugged();
      else displayUnplugged();
    }
    void displayPhotoStatus(bool armedPhoto);
    void displayKeycodeStatus();
    void displayError();
    void displayStatus(bool armedPlug, bool armedAccel, bool armedPhoto, bool plugged);

    void displayAlert(bool isAlarm);
    void endAlert();

    void beepArmed(bool up, bool plugged);
    void beepError() {sound.beepUnauth();}

    void setup();
    void tick();
};

#endif