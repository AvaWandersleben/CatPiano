#include "final_project.h"

void setup() {
  Serial.begin(9600);
  lastTreat = millis() - COOLDOWN;
  stepper.setSpeed(12);
  pinMode(13, OUTPUT);
  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.7);
  digitalWrite(13, HIGH);
  for (int i = 0; i < TOTAL_SWITCHES; i++) {
    ALL_SWITCHES[i]->begin(i+24);
  }
}

void loop() {
  pollAll();
  updatePiano();
  LoaderSwitch.poll();
  if (LoaderSwitch.onPress()) {
    loading = true;
  }
  if (LoaderSwitch.onRelease()) {
    loading = false;
  }


  if (dispense || loading) {
    stepper.step(-3);
    currSteps+=3;
    if (currSteps >= STEPS || loading) {
      currSteps = 0;
      dispense = false;
    }
  }
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

void dispenseTreat() {
  if (millis()-lastTreat >= 1000*COOLDOWN && !loading) {
      lastTreat = millis();
      Serial.println("TREAT DISPENSED!");
        dispense = true;
  }
}

void onNote(int note) {
  dispenseTreat();
  mems[note]->play(samples[note]);
  fades[note]->fadeIn(1);
}

void offNote(int note) {
  fades[note]->fadeOut(5);
}
