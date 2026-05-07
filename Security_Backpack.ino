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
  Security_Backpack.ino
  ---------------------
  Written by Jonas Blackwood
*/

#include "Interface.h"
#include "Scan.h"

constexpr long warningDuration = 10000; // The warning duration in ms

class Backpack {
  private:
  // === PRIVATE VARIABLES ===
  bool armedPlug = 0;
  bool armedAccel = 0;
  bool armedPhoto = 0;
  bool plugged = 0;
  bool authenticated = 0;

  bool photoCalibCirc = 0;
  bool photoCalibPack = 0;
  int darkValPhotoCirc;
  int darkValPhotoPack;

  bool lastPlugged = 1;

  bool warning = 0;
  bool alarm = 0;
  unsigned long warningStartTime = 0;
  bool instantAlarm = 0; 

  bool accelOperational = 1;

  bool keycodeResetPrevCode = 0;
  bool keycodeResetNewCode = 0;

  bool isAlert() { return (warning || alarm); }

  // === MODULES ===
  Interface interface;
  Scan scan;
  Auth auth;

  // === PRIVATE FUNCTIONS ===
  // --- Update Status ---
  void updateStatus() {
    // Update
    plugged = scan.checkPluggedMain();
    // If plug status changed, update display
    if (lastPlugged != plugged && !isAlert()) interface.displayStatus(armedPlug, armedAccel, armedPhoto, plugged);
    lastPlugged = plugged;
    // DEBUG: Print
    // Serial.print("Arm="); Serial.print(armedPlug ? "PLUG" : (armedAccel ? "ACCEL" : "NONE")); if (armedPhoto) Serial.print(" PHOTO"); Serial.print(" Plug="); Serial.print(plugged); Serial.print(" Alert="); Serial.print(isAlert()); if(warning)Serial.print("Warning="); if(warning)Serial.print((millis() - warningStartTime)); if(auth)Serial.print(" AUTH"); Serial.println();
  }
  
  void startWarning(bool _instantAlarm = 0) {
    instantAlarm = _instantAlarm;
    if (instantAlarm) Serial.println("INSTANT ALARM");
    interface.displayAlert(0);
    warning = 1;
    warningStartTime = millis();
  }

  // --- Photo resistors ---
  void startPhotoCalib() { // Start calibrating the photos
    Serial.println("Starting circuit photo calibration...");
    photoCalibCirc = 1; 
    scan.startPhotoSample(PHOTO_CIRC_PIN);
  }

  void updatePhotoCalib() {
    if (photoCalibCirc) {
      bool done = scan.tickPhotoSample();
      if (done) {
        darkValPhotoCirc = scan.getPhotoSampleAvg();
        photoCalibCirc = 0;
        Serial.println("Starting pack photo calibration...");
        photoCalibPack = 1;
        scan.startPhotoSample(PHOTO_PACK_PIN);
      }
    } else if (photoCalibPack) {
      bool done = scan.tickPhotoSample();
      if (done) {
        Serial.println("Photos calibrated.");
        darkValPhotoPack = scan.getPhotoSampleAvg();
        photoCalibPack = 0;
        updateArmedPhoto();
      }
    }
  }

  void updateArmedPhoto() {
    if (!(armedAccel || armedPlug)) {
      Serial.println("Disarmed, aborting pouch arm.");
      return;
    }
    Serial.print("Dark Val Circuit: "); Serial.println(darkValPhotoCirc);
    Serial.print("Dark Val Pack: "); Serial.println(darkValPhotoPack);
    if (darkValPhotoCirc < PHOTO_MINIMUM && darkValPhotoPack < PHOTO_MINIMUM) {
      armedPhoto = 1;
      interface.displayStatus(armedPlug, armedAccel, armedPhoto, plugged);
    } else {
      armedPhoto = 0;
      interface.notifyPackUnsecure();
    }
  }

  // --- Arming & Authentication ---
  void updateAuth() {
    if (interface.checkKeycodeIn()) {
      char* keycode = interface.getKeycode();
      Serial.print("Keycode in: "); Serial.println(keycode);

      bool special = checkSpecialCodes(keycode);
      authenticated = auth.checkKeycode(keycode);
      interface.clearKeycode();
      Serial.print("Special="); Serial.println(special);

      if (!special) {
        if (!authenticated) {
          interface.notifyUnauth();
          keycodeResetPrevCode = 0; // If not auth, end keycode reset
          keycodeResetNewCode = 0;
        } else {
          Serial.println("AUTHENTICATED");
        }
      }
    }
  }

  // Runs when authenticated and arm needs to toggle
  void updateArmed() {
    if (!armedAccel && (armedPlug || plugged)) { // If plugged in or secured by plug
      armedPlug = !armedPlug;
      interface.beepArmed(armedPlug, 1);
    } else if (armedAccel || !plugged) { // If on battery or secured by accelerometer
      if (accelOperational) {
        armedAccel = !armedAccel;
        interface.beepArmed(armedAccel, 0);
        // if (armedAccel) interface.notifyAccel();
      } else {
        interface.notifyAccelError();
      }
    }
    // Photo resistors
    Serial.print(armedAccel); Serial.print(" "); Serial.print(armedPlug);
    if (armedAccel || armedPlug) { // If it is armed
      startPhotoCalib();
    } else {
      armedPhoto = 0;
    }

    if(!(armedPlug || armedAccel)) interface.endNotify();
    interface.displayStatus(armedPlug, armedAccel, armedPhoto, plugged);
  }

  // --- Update Alert ---
  // Check if any triggers need to fire alarm
  void updateAlert() {
    // Check for armed plug or armed accel violation
    if (!isAlert()) {
      if (armedPlug && !plugged) {
        startWarning();
      } else if (armedAccel) {
        if (scan.checkAccelMovement()) {
          startWarning();
          Serial.println("ACCELEROMETER MOVEMENT!");
        }
      }
    }

    // Check for photo violation
    if(armedPhoto && !alarm) {
      bool circOpen = scan.checkPhotoLight(PHOTO_CIRC_PIN, darkValPhotoCirc);
      bool packOpen = scan.checkPhotoLight(PHOTO_PACK_PIN, darkValPhotoPack);
      if (circOpen || (packOpen && !warning)) {
        startWarning(circOpen ? 1 : 0); // Sound alarm instantly if circuitry is open
        Serial.println("PHOTO OPENED!");
      }
    }
    
    // Check if loud alarm needs to sound
    if (warning) if (instantAlarm || (millis() - warningStartTime > warningDuration)) {
      Serial.println("STARTING FULL ALARM!");
      warning = 0;
      alarm = 1;
      interface.displayAlert(1); // Sound full alarm
    }
    
  }

  // --- End Alert ---
  void endAlert() {
    alarm = 0;
    warning = 0;
    interface.endAlert();
  }

  // --- Special Codes ---
  bool checkSpecialCodes(char* keycode) { // Return 1 blocks default authentication behavior
    if (armedAccel || armedPlug || armedPhoto || isAlert()) return 0; // No special codes when armed

    if (interface.keycodesAreEqual(keycode, "AAAA")) {
      startKeycodeReset();
      return 1;
    } else if (keycodeResetPrevCode) {
      if (auth.checkKeycode(keycode)) {
        startAuthKeycodeReset();
        return 1;
      }
      keycodeResetPrevCode = 0;
    } else if (keycodeResetNewCode) {
      finishKeycodeReset(keycode);
      return 1;
    }
    return 0;
  }

  void startKeycodeReset() {
    Serial.println("KEYCODE RESET");
    interface.displayKeycodePrevCode();
    keycodeResetPrevCode = 1;
  }

  void startAuthKeycodeReset() {
    Serial.println("Keycode reset: Authenticated.");
    interface.displayKeycodeNewCode();
    keycodeResetNewCode = 1;
    keycodeResetPrevCode = 0;
  }

  void finishKeycodeReset(char* keycode) {
    interface.clearKeycode();
    if (!auth.resetKeycode(keycode)) { // If reset errors
      interface.beepError();
      interface.notifyKeycodeResetError();
    } else {
      interface.notifyKeycodeResetSuccess();
    }
    keycodeResetPrevCode = 0;
    keycodeResetNewCode = 0;
    interface.displayStatus(armedPlug, armedAccel, armedPhoto, plugged);
  }



  public:
  // === SETUP & TICK ===
  void setup() {
    Serial.println("Starting Security Backpack!");

    int error = scan.setup();
    interface.registerError(error);
    if (error == ERROR_ACCEL_NOT_CONNECTED) accelOperational = 0;
    interface.setup();
    auth.setup();

    interface.displayStatus(armedPlug, armedAccel, armedPhoto, lastPlugged);
    interface.endAlert();
  }

  void tick() {
    scan.tick();
    interface.tick();

    updateAuth();
    updateStatus();
    updatePhotoCalib();
    updateAlert();
    
    if (authenticated) {
      authenticated = 0;
      if (isAlert()) endAlert();
      updateArmed();
    }
  }
};


Backpack backpack;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 100 && !Serial; i++) {
    delay(10);
  }
  backpack.setup();
}

void loop() {
  backpack.tick();
  // Serial.print(analogRead(PHOTO_CIRC_PIN)); Serial.print(" "); Serial.println(analogRead(PHOTO_PACK_PIN));


  delay(50);
}
