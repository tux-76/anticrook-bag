#include <LiquidCrystal.h>
#include "Sound.h"
#include "Interface.h"
#include "Scan.h"
#include "Keypad.h"

constexpr long warningDuration = 20000; // The warning duration in ms

class Backpack {
  private:
  // === PRIVATE VARIABLES ===
  bool armedPlug = 0;
  bool armedAccel = 0;
  bool armedPhoto = 0;
  bool plugged = 0;
  bool auth = 0;

  bool lastPlugged = 1;

  bool warning = 0;
  bool alarm = 0;
  unsigned long warningStartTime = 0;

  bool accelOperational = 1;

  bool isAlert() { return (warning || alarm); }

  // === MODULES ===
  Interface interface;
  Scan scan;

  // === PRIVATE FUNCTIONS ===
  // --- Update Status ---
  void updateStatus() {
    // Update
    auth = interface.checkAuthenticated();
    plugged = scan.checkPluggedMain();
    // If plug status changed, update display
    if (lastPlugged != plugged && !isAlert()) interface.displayStatus(armedPlug, armedAccel, armedPhoto, plugged);
    lastPlugged = plugged;
    // DEBUG: Print
    // Serial.print("Arm="); Serial.print(armedPlug ? "PLUG" : (armedAccel ? "ACCEL" : "NONE")); Serial.print(" Plug="); Serial.print(plugged); Serial.print(" Alert="); Serial.print(isAlert()); if(warning)Serial.print("Warning="); if(warning)Serial.print((millis() - warningStartTime)); if(auth)Serial.print(" AUTH"); Serial.println();
  }
  
  void startWarning() {
    interface.displayAlert(0);
    warning = 1;
    warningStartTime = millis();
  }

  // --- Update Armed ---
  // Runs when authenticated and arm needs to toggle
  void updateArmed() {
    if (!armedAccel && (armedPlug || plugged)) { // If plugged in or secured by plug
      armedPlug = !armedPlug;
      interface.displayStatus(armedPlug, armedAccel, armedPhoto, plugged);
      interface.armPlugBeep(armedPlug);
    } else if (armedAccel || !plugged) { // If on battery or secured by accelerometer
      if (accelOperational) {
        armedAccel = !armedAccel;
        interface.displayStatus(armedPlug, armedAccel, armedPhoto, plugged);
        interface.armPlugBeep(armedAccel);
        if (armedAccel) interface.notifyAccel();
      } else {
        interface.notifyAccelError();
      }
    }
  }

  // --- Update Alert ---
  // Check if any triggers need to fire alarm
  void updateAlert() {
    // Check for armed plug violation
    if (armedPlug && !plugged && !isAlert()) {
      startWarning();
    } else if (armedAccel && !isAlert()) {
      if (scan.checkAccelMovement()) {
        startWarning();
        Serial.println("ACCELEROMETER MOVEMENT!");
      }
    }

    // Check if loud alarm needs to sound
    if (warning) if (millis() - warningStartTime > warningDuration) {
      Serial.println("STARTING FULL ALARM");
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

  public:
  // === SETUP & TICK ===
  void setup() {
    int error = scan.setup();
    interface.registerError(error);
    if (error == ERROR_ACCEL_NOT_CONNECTED) accelOperational = 0;
    interface.setup();
    interface.displayStatus(armedPlug, armedAccel, armedPhoto, lastPlugged);
    interface.endAlert();
  }

  void tick() {
    scan.tick();
    interface.tick();

    updateStatus();
    updateAlert();
    
    if (auth) {
      if (isAlert()) endAlert();
      updateArmed();
    }
  }
};


Backpack backpack;
#include "Accelerometer.h"
Accelerometer accel;

void setup() {
  Serial.begin(9600);
  backpack.setup();
  // accel.setup();
  delay(2000);
}

void loop() {
  backpack.tick();
  // accel.tick();
  // Serial.println(accel.calcMovement());
  delay(50);
}
