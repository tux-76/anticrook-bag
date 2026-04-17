#include "Interface.h"

// === INTERFACE ===
// --- Contstructor ---
// Like the setup() function of the interface;
void Interface::setup() {
  lcd.begin(16, 2);
  lcd.createChar(SYMBOL_LOCKED, SYMBOL_LOCKED_IMG);
  lcd.createChar(SYMBOL_UNLOCKED, SYMBOL_UNLOCKED_IMG);
  lcd.createChar(SYMBOL_WARNING, SYMBOL_WARNING_IMG);
  lcd.createChar(SYMBOL_BATT, SYMBOL_BATT_IMG);
  lcd.createChar(SYMBOL_PLUG, SYMBOL_PLUG_IMG);
  lcd.clear();
  displayDisarmed();
}

// --- Display ---

void Interface::displayArmed() {
  lcd.setCursor(0, 0);
  lcd.write(SYMBOL_LOCKED);
  lcd.print(" SECURE ");
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

void Interface::displayKeycodeStatus() {
  lcd.setCursor(0, 1);
  lcd.print("     ");
  lcd.setCursor(0, 1);
  for (int i = 0; i < keycodeLen; i++) lcd.print("*");
}

void Interface::displayStatus(bool armed, bool plugged, bool armBeep) {
  lcd.clear();
  displayArmStatus(armed);
  displayPlugStatus(plugged);
  displayKeycodeStatus();
  if (armBeep) {
    if (armed) sound.beepArmed();
    else sound.beepDisarmed();
  }
}

void Interface::displayAlert() {
  sound.soundAlarm();

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
  sound.endAlarm();
}

void Interface::checkKeycode() {
  authenticated = 1;
  for (int i = 0; i < KEYCODE_LEN; i++) {
    if (keycode[i] != TRUE_CODE[i]) authenticated = 0;
  }
  if (!authenticated) sound.beepUnauth();
}

void Interface::checkKeyPressed() {
  char key = keypad.getKeyPressed();
  if (key) {
    sound.beepKey();
    if (keycodeLen < KEYCODE_LEN) { // If code isn't complete
      keycode[keycodeLen] = key;
      keycodeLen++;
    }
    if (keycodeLen >= KEYCODE_LEN) {
      checkKeycode();
      keycodeLen = 0;
    }
    displayKeycodeStatus();
    Serial.println(keycode);
  }
  
  bool buttonIn = digitalRead(BUTTON_PIN);
  if (buttonIn && !lastButtonInput) {
    checkKeycode();
  }
  lastButtonInput = buttonIn;
}

bool Interface::checkAuthenticated() {
  if (authenticated) {
    authenticated = 0;
    return 1;
  } else return 0;
}

void Interface::tick() {
  sound.tick();

  checkKeyPressed();
 
}

