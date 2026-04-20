#include <LiquidCrystal.h>
#include "Sound.h"
#include "Interface.h"
#include "Scan.h"
#include "Keypad.h"



class Backpack {
  private:
  // === PRIVATE VARIABLES ===
  bool armed = 0;
  bool alert = 0;
  bool plugged = 0;
  bool auth = 0;

  bool lastPlugged = 1;

  // -=== MODULES ===
  Interface interface;
  

  // === PRIVATE FUNCTIONS ===
  // --- Update Status ---
  void updateStatus() {
    // Update
    auth = interface.checkAuthenticated();
    plugged = checkPluggedMain();
    // If plug status changed, update display
    if (lastPlugged != plugged && !alert) interface.displayStatus(armed, plugged);
    lastPlugged = plugged;
    // DEBUG: Print
    // Serial.print("Arm="); Serial.print(armed); Serial.print(" Plug="); Serial.print(plugged); Serial.print(" Alert="); Serial.print(alert); Serial.print(auth ? " AUTH" : ""); Serial.println();
  }
  
  // --- Update Alert ---
  void updateAlert() {
    if (armed && !plugged && !alert) {
      alert = 1;
      interface.displayAlert();
    } 
  }

  // --- Update Armed ---
  void updateArmed() {
    if (armed || plugged) {
      armed = !armed;
      interface.displayStatus(armed, plugged, true);
    } else if (!plugged) {
      interface.notifyOnBatt();
    }
  }

  // --- End Alert ---
  void endAlert() {
    alert = 0;
    interface.endAlert();
  }

  public:
  // === SETUP & TICK ===
  void setup() {
    interface.setup();
    interface.displayStatus(armed, lastPlugged);
    interface.endAlert();
  }

  void tick() {
    interface.tick();

    updateStatus();
    updateAlert();
    
    if (auth) {
      if (alert) endAlert();
      updateArmed();
    }
  }
};


Backpack backpack;

void setup() {
  Serial.begin(9600);
  backpack.setup();
  delay(2000);
}

void loop() {
  backpack.tick();
  delay(50);

}
