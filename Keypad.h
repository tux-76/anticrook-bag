#ifndef KEYPAD_H
#define KEYPAD_H

#define KEYPAD_R1    5
#define KEYPAD_R2    4
#define KEYPAD_R3    3
#define KEYPAD_R4    2
#define KEYPAD_C1    9
#define KEYPAD_C2    8
#define KEYPAD_C3    7
#define KEYPAD_C4    6

#define ROWS 4
#define COLS 4
int ROW_PINS[ROWS] = {KEYPAD_R1, KEYPAD_R2, KEYPAD_R3, KEYPAD_R4};
int COL_PINS[COLS] = {KEYPAD_C1, KEYPAD_C2, KEYPAD_C3, KEYPAD_C4};

char KEYMAP[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};

class Keypad {
  private:
    bool keyHeld = 0;
  public:
    void setup();
    char getKeyPressed();
};

#endif 