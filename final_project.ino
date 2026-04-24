#include "final_project.h"

void setup() {
  Serial.begin(9600);
  SD.begin(BUILTIN_SDCARD);
  setSyncProvider(Teensy3Clock.get);
  lastTreat = millis() - 1000*COOLDOWN;
  pinMode(13, OUTPUT);
  AudioMemory(400);
  queue1.begin();
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.7);
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  pinMode(leftMotor, OUTPUT);
  pinMode(rightMotor, OUTPUT);

  digitalWrite(leftMotor, LOW);
  digitalWrite(rightMotor, LOW);
}

void loop() {
  if (recording && ((lastNoteTime + 10000) < millis())) {
    recording = false;
    addToSD();
    writeWavHeader();
    currRecording.close();
    Serial.println("sd closed");
  }

  if (recording) {
    addToSD();
  }

  pollAll();
  updatePiano();
  LoaderSwitch.poll();
  if (LoaderSwitch.onPress()) {
    dispense = true;
    digitalWrite(rightMotor, HIGH);
    digitalWrite(leftMotor, LOW);
    motorStart = millis();
  }

  if (dispense && (motorStart + 1000 < millis())) {
    digitalWrite(leftMotor, LOW);
    digitalWrite(rightMotor, LOW);
    dispense = false;
  }

  analog.update();
  int input = analog.getValue();
  if (analog.hasChanged()) {
    currLevel = map(input, 0, 1023, 9, 1);
  }
  displayNumber(currLevel);
}

void pollAll(){
  for (int i = 0; i < TOTAL_SWITCHES; i++) {
    ALL_SWITCHES[i]->poll();
  }
}

void updatePiano() {
  for (int i = 0; i < TOTAL_SWITCHES; i++) {
    if (ALL_SWITCHES[i]->onPress()) {
      onNote(i);
    } else if (ALL_SWITCHES[i]->onRelease()) {
      offNote(i);
    }
  }
}

void dispenseTreat(int currNote) {
  if ((currLevel == 1) || ((lastNote + 1 == currNote) && currNote + 2 == currLevel)) {
    lastNote = currNote;
    if (millis()-lastTreat >= 1000*COOLDOWN && !dispense) {
        lastTreat = millis();
        motorStart = millis();
        digitalWrite(rightMotor, HIGH);
        digitalWrite(leftMotor, LOW);
        Serial.println("TREAT DISPENSED!");
        dispense = true;
        lastNote = -1;
    }
  } else {
      if (currNote == 0) {
        lastNote = 0;
      } else if (lastNote + 1 == currNote) {
        lastNote = currNote;
      } else {
        lastNote = -1;
      }
  }
}

void onNote(int note) {
  dispenseTreat(note);
  mems[note]->play(samples[note]);
  fades[note]->fadeIn(1);
  if (!recording) {
    time_t currTime = Teensy3Clock.get();
    String dateString = String(month(currTime)) + " " + String(day(currTime)) + ".wav";
    currRecording = SD.open(dateString.c_str(), FILE_WRITE);
    if (currRecording) {
      Serial.println("sd open");
      byte headerDummy[44] = {0};
      currRecording.write(headerDummy, 44);
      recording = true;
    }
  }
  lastNoteTime = millis();
}

void offNote(int note) {
  fades[note]->fadeOut(5);
}

void addToSD() {
  while (queue1.available() >= 1) {
    byte buffer[256];
    memcpy(buffer, queue1.readBuffer(), 256);
    queue1.freeBuffer();
    currRecording.write(buffer, 256);
  }
}
