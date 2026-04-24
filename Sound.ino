#include "Sound.h"
#include <cassert>

// --- Note manager ---
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
    if (isAlert()) alarmInterrupted = 1;
  } else {
    // Serial.print(warning); Serial.println(alarm);
    if (!isAlert()) noTone(PIEZO_PIN);
    else if (alarmInterrupted) {
      if (alarm) soundAlarmTone();
      else soundWarningTone();
      alarmInterrupted = 0;
    }
  }
}

// --- Warning ---
void Sound::soundWarning() {
  warning = 1;
  alarmTick = millis();
  alarmHigh = 1;
  soundWarningTone();
}

void Sound::soundWarningTone() {
  if (alarmHigh) tone(PIEZO_PIN, 2000);
  else tone(PIEZO_PIN, 1300);
}

void Sound::endWarning() {
  warning = 0;
  alarmInterrupted = 0;
  noTone(PIEZO_PIN);
}

// --- Alarm ---
void Sound::soundAlarm() {
  alarm = 1;
  alarmTick = millis();
  alarmHigh = 1;
  soundAlarmTone();
}

void Sound::soundAlarmTone() {
  if (alarmHigh) tone(ALARM_PIN, 2000);
  else tone(ALARM_PIN, 1300);
}

void Sound::endAlarm() {
  alarm = 0;
  alarmInterrupted = 0;
  noTone(ALARM_PIN);
}

// --- Beeps ---
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

// --- Tick ---
void Sound::tick() {
  unsigned long time = millis();
  unsigned long noteTickDur = time - noteTick;
  unsigned long alarmTickDur = time - alarmTick;
  if (isAlert()) {
    if (alarmTickDur > ALARM_BEEP_DUR) {
      alarmHigh = !alarmHigh;
      if (warning) soundWarningTone();
      else soundAlarmTone();

      alarmTick = time;
    }
  }
  
  if (noteTickDur > FEEDBACK_BEEP_DUR) {
    playNextNote();
  }
}