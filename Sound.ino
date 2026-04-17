#include "Sound.h"
#include <cassert>

void Sound::pushNote(int note) {
  if (numNotes < MAX_QUEUE_NOTES) { 
    notes[numNotes] = note;
    numNotes += 1;
  }
}

int Sound::popNote() {
  assert(numNotes > 0);
  numNotes -= 1;
  int note = notes[0];
  // Push other notes
  for (int i = 0; i < numNotes; i++) {
    notes[i] = notes[i+1];
  }
  return note;
}

void Sound::playNextNote() {
  if (alarm) return;

  changeTime = millis();
  if (numNotes > 0) {
    int note = popNote();
    tone(ALARM_PIN, note);
  } else {
    noTone(ALARM_PIN);
  }
}

void Sound::tick() {
  unsigned long time = millis();
  unsigned long timePassed = time - changeTime;
  if (alarm) {
    if (timePassed > ALARM_BEEP_DUR) {
      alarmHigh = !alarmHigh;
      soundAlarmTone();

      changeTime = time;
    }
  }
  else if (timePassed > FEEDBACK_BEEP_DUR) {
    playNextNote();
  }
}

void Sound::soundAlarm() {
  alarm = 1;
}

void Sound::endAlarm() {
  alarm = 0;
  noTone(ALARM_PIN);
}

void Sound::beepArmed() {
  pushNote(NOTE_G5);
  pushNote(NOTE_C6);
  playNextNote();
}

void Sound::beepDisarmed() {
  numNotes = 0; // May need better solution, but keeps out built up notes during alarm
  pushNote(NOTE_G5);
  pushNote(NOTE_C5);
  playNextNote();
}

void Sound::beepKey() {
  pushNote(NOTE_F5);
  playNextNote();
}

void Sound::beepUnauth() {
  pushNote(NOTE_F5);
  pushNote(NOTE_F2);
  pushNote(NOTE_REST);
  pushNote(NOTE_F2);
  playNextNote();
}