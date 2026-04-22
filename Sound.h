#ifndef SOUND_H
#define SOUND_H

#include "Music_Notes.h"

#define MAX_QUEUE_NOTES 10

constexpr int PIEZO_PIN = 23;
constexpr int ALARM_BEEP_DUR = 500; // in millis
constexpr int FEEDBACK_BEEP_DUR = 100;

class Sound {
  private:
    unsigned long noteTick = 0;
    unsigned long alarmTick = 0;
    bool warning = 0;
    bool alarmHigh = 0;
    bool alarmInterrupted = 0;

    int notes[MAX_QUEUE_NOTES] = {};
    int numNotes = 0;
    void pushNote(int note);
    int popNote();
    void playNextNote();

    bool notesEmpty() {
      return (numNotes < 1);
    }

    void soundWarningTone() {
      if (alarmHigh) tone(PIEZO_PIN, 2000);
      else tone(PIEZO_PIN, 1300);
    }

  public:
    void soundWarning();
    void endWarning();
    void beepArmed();
    void beepDisarmed();
    void beepKey();
    void beepUnauth();
    void tick();
};


#endif