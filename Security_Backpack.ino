#include <LiquidCrystal.h>
#include "Sound.h"
#include "Interface.h"
#include "Scan.h"
#include "Keypad.h"

constexpr long warningDuration = 20000; // The warning duration in ms

class Backpack {
  private:
  // === PRIVATE VARIABLES ===
  bool armed = 0;
  bool plugged = 0;
  bool auth = 0;

  bool lastPlugged = 1;

  bool warning = 0;
  bool alarm = 0;
  unsigned long warningStartTime = 0;

  bool isAlert() { return (warning || alarm); }

  // === MODULES ===
  Interface interface;
  

  // === PRIVATE FUNCTIONS ===
  // --- Update Status ---
  void updateStatus() {
    // Update
    auth = interface.checkAuthenticated();
    plugged = checkPluggedMain();
    // If plug status changed, update display
    if (lastPlugged != plugged && !isAlert()) interface.displayStatus(armed, plugged);
    lastPlugged = plugged;
    // DEBUG: Print
    // Serial.print("Arm="); Serial.print(armed); Serial.print(" Plug="); Serial.print(plugged); Serial.print(" Alert="); Serial.print(isAlert()); if(warning)Serial.print("Warning="); if(warning)Serial.print((millis() - warningStartTime)); if(auth)Serial.print(" AUTH"); Serial.println();
  }
  
  void startWarning() {
    interface.displayAlert(0);
    warning = 1;
    warningStartTime = millis();
  }

  // --- Update Alert ---
  void updateAlert() {
    // Serial.println(millis() - warningStartTime);
    if (warning) if (millis() - warningStartTime > warningDuration) {
      Serial.println("STARTING FULL ALARM");
      warning = 0;
      alarm = 1;
      interface.displayAlert(1); // Sound full alarm
    }
    if (armed && !plugged && !isAlert()) {
      startWarning();
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
    alarm = 0;
    warning = 0;
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
      if (isAlert()) endAlert();
      updateArmed();
    }
  }
};


Backpack backpack;

// --- DEBUG ---
Sound sound;
unsigned long changeTime = 0;
bool alarm = 0;

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// --- END DEBUG ---
void setup() {
  Serial.begin(9600);
  backpack.setup();
  // sound.soundWarning();
  delay(2000);
}

void loop() {
  backpack.tick();
  
  // sound.tick();
  // unsigned long time = millis();
  // Serial.println(time - changeTime);
  // if (time - changeTime > warningDuration && !alarm) {
  //   Serial.println("Ending warning!");
  //   delay(1000);
  //   alarm = 1;
  //   sound.endWarning();
  //   sound.soundAlarm();
  // }

  delay(50);

}
