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
  Sound.h
  ---------------------
  Written by Jonas Blackwood
*/

#ifndef SOUND_H
#define SOUND_H

#include "Music_Notes.h"

#define MAX_QUEUE_NOTES 10

#define ALARM_PIN A12

constexpr int PIEZO_PIN = 23;
constexpr int ALARM_BEEP_DUR = 500; // in millis
constexpr int FEEDBACK_BEEP_DUR = 100;

class Sound {
  private:
    unsigned long noteTick = 0;
    unsigned long alarmTick = 0;
    bool warning = 0;
    bool alarm = 0;
    bool alarmHigh = 0;
    bool alarmInterrupted = 0;

    bool isAlert() { return (alarm || warning); }

    int notes[MAX_QUEUE_NOTES] = {};
    int numNotes = 0;
    void pushNote(int note);
    int popNote();
    void playNextNote();

    bool notesEmpty() {
      return (numNotes < 1);
    }

    void soundWarningTone();
    void soundAlarmTone();

  public:
    void soundWarning();
    void endWarning();
    void soundAlarm();
    void endAlarm();
    void beepArmed();
    void beepDisarmed();
    void beepKey();
    void beepUnauth();
    void tick();
};


#endif