#ifndef INTERFACE_H
#define INTERFACE_H

#include "LCD_Symbols.h"
#include "Keypad.h"

// === LCD SCREEN ===
constexpr int LCD_RS=52, LCD_EN=53, LCD_D4=48, LCD_D5=49, LCD_D6=50, LCD_D7=51;
// constexpr int LCD_RS=52, LCD_EN=53, LCD_D4=48, LCD_D5=49, LCD_D6=50, LCD_D7=51;
constexpr int BUTTON_PIN = 22;

constexpr int KEYCODE_LEN = 4;
const char TRUE_CODE[KEYCODE_LEN] = {'1', '2', '3', '4'};

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
    void checkKeycode();
    void checkKeyPressed();

    void tickNotify();

    bool _armed = 0;    // Varialbles for if data needs to be rewritten
    bool _plugged = 0;

  public:
    Interface() : lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7) {
      pinMode(BUTTON_PIN, INPUT);
    };

    bool checkAuthenticated();
    void notifyUnauth();
    void notifyOnBatt();

    void displayArmed();
    void displayDisarmed();
    void displayArmStatus(bool armed) {
      if (armed) displayArmed();
      else displayDisarmed();
    }
    void displayPlugged();
    void displayUnplugged();
    void displayPlugStatus(bool plugged) {
      if (plugged) displayPlugged();
      else displayUnplugged();
    }
    void displayKeycodeStatus();
    void displayStatus(bool armed, bool plugged, bool armBeep = false);

    void displayAlert();
    void endAlert();

    void setup();
    void tick();
};

#endif