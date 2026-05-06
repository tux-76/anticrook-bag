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
  Interface.ino
  ---------------------
  Written by Jonas Blackwood
*/

#include "Interface.h"

// === INTERFACE ===
// --- Constructor ---
// Like the setup() function of the interface;
void Interface::setup() {
  lcd.begin(16, 2);
  lcd.createChar(SYMBOL_LOCKED, SYMBOL_LOCKED_IMG);
  lcd.createChar(SYMBOL_UNLOCKED, SYMBOL_UNLOCKED_IMG);
  lcd.createChar(SYMBOL_WARNING, SYMBOL_WARNING_IMG);
  lcd.createChar(SYMBOL_BATT, SYMBOL_BATT_IMG);
  lcd.createChar(SYMBOL_PLUG, SYMBOL_PLUG_IMG);
  lcd.createChar(SYMBOL_PHOTO, SYMBOL_PHOTO_IMG);
  lcd.clear();
  displayDisarmed();
}

void Interface::registerError(int code) {
  backpackError = code;
}

// === DISPLAY ===
// --- Notify ---
void Interface::notifyAccelError() {
  Serial.println("Interface Notify: On Batt");
  notifyTime = 50;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ACCEL ERROR :(");
  lcd.setCursor(0, 1);
  lcd.print("Cannot secure ");
  lcd.write(SYMBOL_UNLOCKED);
}

void Interface::notifyUnauth() {
  Serial.println("Interface Notify: Unauthorized");
  sound.beepUnauth();
  notifyTime = 40;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("INVALID CODE");
}

void Interface::notifyAccel() {
  Serial.println("Interface Notify: Accelerometer");
  notifyTime = 100;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Secured with");
  lcd.setCursor(0, 1);
  lcd.print("Accelerometer ");
  lcd.write(SYMBOL_LOCKED);
}

void Interface::notifyPackUnsecure() {
  Serial.println("Interface Notify: Pack unsecure");
  notifyTime = 100;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(SYMBOL_WARNING);
  lcd.print(" ");
  lcd.print("Pouch Open ");
  lcd.write(SYMBOL_PHOTO);
  lcd.setCursor(0, 1);
  lcd.print("Pouch Insecure ");
  lcd.write(SYMBOL_UNLOCKED);
}

void Interface::notifyKeycodeResetError() {
  Serial.println("Interface Notify: Keycode not reset");
  notifyTime = 100;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(SYMBOL_WARNING);
  lcd.print(" ");
  lcd.print("KEYCODE RESET");
  lcd.setCursor(0, 1);
  lcd.print("FAILURE");
}

void Interface::notifyKeycodeResetSuccess() {
  Serial.println("Interface Notify: Keycode Reset");
  notifyTime = 100;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("KEYCODE RESET");
  lcd.setCursor(0, 1);
  lcd.print("SUCCESS ");
  lcd.write(SYMBOL_LOCKED);
}


void Interface::endNotify() {
  notifyTime = 0;
  Serial.println("Notification reset.");
  displayStatus(_armedPlug, _armedAccel, _armedPhoto, _plugged);
  displayKeycodeStatus();
}

// --- Dialoge ---
void Interface::displayKeycodePrevCode() {
  notifyTime = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Current Code:");
}

void Interface::displayKeycodeNewCode() {
  notifyTime = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("New Code:");
}

// --- Display Status ---
void Interface::displayArmedPlug() {
  lcd.setCursor(0, 0);
  lcd.write(SYMBOL_LOCKED);
  lcd.print(" SECURE ");
}

void Interface::displayArmedAccel() {
  lcd.setCursor(0, 0);
  lcd.write(SYMBOL_LOCKED);
  lcd.print(" ACCEL ");
}

void Interface::displayDisarmed() {
  lcd.setCursor(0, 0);
  lcd.write(SYMBOL_UNLOCKED);
  lcd.print(" OFF");
}

void Interface::displayPlugged() {
  lcd.setCursor(9, 0);
  lcd.write(SYMBOL_PLUG);
  lcd.print(" PLUG");
}

void Interface::displayUnplugged() {
  lcd.setCursor(9, 0);
  lcd.write(SYMBOL_BATT);
  lcd.print(" BATT");
}

void Interface::displayPhotoStatus(bool armedPhoto) {
  if (armedPhoto) {
    lcd.setCursor(9, 1);
    lcd.write(SYMBOL_PHOTO);
    lcd.print(" POUCH");
  }
}

void Interface::displayKeycodeStatus() {
  Serial.println(keycodeLen);
  lcd.setCursor(0, 1);
  lcd.print("     ");
  lcd.setCursor(0, 1);
  for (int i = 0; i < keycodeLen; i++) lcd.print("*");
}

void Interface::displayError() {
  if (backpackError == ERROR_ACCEL_NOT_CONNECTED) {
    lcd.setCursor(12, 1);
    lcd.write(SYMBOL_WARNING);
    lcd.print("ACL");
  }
}

void Interface::displayStatus(bool armedPlug, bool armedAccel, bool armedPhoto, bool plugged) {
  _armedPlug = armedPlug; _armedAccel = armedAccel; _armedPhoto = armedPhoto; _plugged = plugged;
  if (notifyTime > 0) return;
  // Serial.println("DP");
  lcd.clear();
  displayArmStatus(armedPlug, armedAccel);
  displayPlugStatus(plugged);
  displayPhotoStatus(armedPhoto);
  displayKeycodeStatus();
  displayError();
}

void Interface::armPlugBeep(bool up) {
  if (up) sound.beepArmed();
  else sound.beepDisarmed();
}

void Interface::displayAlert(bool isAlarm) {
  if (isAlarm) {
    sound.endWarning();
    sound.soundAlarm();
  } else {
    sound.soundWarning();
  }

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.write(SYMBOL_WARNING);
  lcd.print(" ");
  lcd.write(SYMBOL_WARNING);
  lcd.print(" ALERT! ");
  lcd.write(SYMBOL_WARNING);
  lcd.print(" ");
  lcd.write(SYMBOL_WARNING);
}

void Interface::endAlert() {
  sound.endWarning();
  sound.endAlarm();
}

// --- Keypad & Keycodes --- 
bool Interface::checkKeycodeIn() {
  if (keycodeLen > KEYCODE_LEN) Serial.println("WARNING: Checking too long of a keycode!");
  return (keycodeLen >= KEYCODE_LEN);
}

char* Interface::getKeycode () {
  return keycode;
}

void Interface::clearKeycode() {
  keycodeLen = 0;
}

bool Interface::keycodesAreEqual(char *code1, char *code2) {
  for (int i = 0; i < KEYCODE_LEN; i++) {
    if (code1[i] != code2[i]) return 0;
  }
  return 1;
}

void Interface::checkKeyPressed() {
  char key = keypad.getKeyPressed();
  if (key) {
    sound.beepKey();
    if (keycodeLen < KEYCODE_LEN) { // If code isn't complete
      keycode[keycodeLen] = key;
      keycodeLen++;
    } else Serial.print("WARNING: Keycode too long!");
    displayKeycodeStatus();
  }
}

// --- Tick ---
void Interface::tickNotify() {
  if (notifyTime > 1) {
    notifyTime--;
  } else if (notifyTime == 1) {
    endNotify();
  }
}

void Interface::tick() {
  sound.tick();
  tickNotify();
  checkKeyPressed();
 
}

