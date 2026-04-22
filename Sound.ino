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
  noteTick = millis();
  if (numNotes > 0) {
    int note = popNote();
    tone(PIEZO_PIN, note);
    if (warning) alarmInterrupted = 1;
  } else {
    if (!warning) noTone(PIEZO_PIN);
    else if (alarmInterrupted) {
      soundWarningTone();
      alarmInterrupted = 0;
    }
  }
}

void Sound::tick() {
  unsigned long time = millis();
  unsigned long noteTickDur = time - noteTick;
  unsigned long alarmTickDur = time - alarmTick;
  if (warning) {
    if (alarmTickDur > ALARM_BEEP_DUR) {
      alarmHigh = !alarmHigh;
      soundWarningTone();

      alarmTick = time;
    }
  }
  
  if (noteTickDur > FEEDBACK_BEEP_DUR) {
    playNextNote();
  }
}

void Sound::soundWarning() {
  warning = 1;
  alarmTick = millis();
  alarmHigh = 1;
  soundWarningTone();
}

void Sound::endWarning() {
  warning = 0;
  noTone(PIEZO_PIN);
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