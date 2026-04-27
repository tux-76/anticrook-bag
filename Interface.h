#ifndef INTERFACE_H
#define INTERFACE_H

#include "LCD_Symbols.h"
#include "Keypad.h"
#include "Scan.h" // For error codes

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

    bool checkAuthenticated();
    void notifyUnauth();
    void notifyAccelError(); // Depricated
    void notifyAccel();

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
    void displayKeycodeStatus();
    void displayError();
    void displayStatus(bool armedPlug, bool armedAccel, bool armedPhoto, bool plugged);

    void displayAlert(bool isAlarm);
    void endAlert();

    void armPlugBeep(bool up);

    void setup();
    void tick();
};

#endif