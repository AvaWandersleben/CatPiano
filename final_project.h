#ifndef FINALPROJECT
#define FINALPROJECT

#include <Toggle.h>
#include <math.h>
#include <TimeLib.h>
#include <ResponsiveAnalogRead.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "AudioSampleA1.h"
#include "AudioSampleB1.h"
#include "AudioSampleC1.h"
#include "AudioSampleD1.h"
#include "AudioSampleE1.h"
#include "AudioSampleF1.h"
#include "AudioSampleG1.h"
#include "AudioSampleC2.h"

// GUItool: begin automatically generated code
AudioPlayMemory          playMem6;       //xy=116,458
AudioPlayMemory          playMem7;       //xy=121,521
AudioPlayMemory          playMem1;       //xy=135,43
AudioPlayMemory          playMem4;       //xy=136,295
AudioPlayMemory          playMem2;       //xy=143,117
AudioPlayMemory          playMem5;       //xy=148,377
AudioPlayMemory          playMem8;       //xy=154,585
AudioPlayMemory          playMem3;       //xy=160,212
AudioEffectFade          fade1;          //xy=282,48
AudioEffectFade          fade2;          //xy=319,110
AudioEffectFade          fade3;          //xy=335,203
AudioEffectFade          fade4;          //xy=366,290
AudioEffectFade          fade7;          //xy=367,528
AudioEffectFade          fade6;          //xy=380,461
AudioEffectFade          fade5;          //xy=388,367
AudioEffectFade          fade8;          //xy=391,582
AudioMixer4              mixer1;         //xy=595,148
AudioMixer4              mixer2;         //xy=594,450
AudioMixer4              mixer3;         //xy=775,290
AudioRecordQueue         queue1;         //xy=1089,394
AudioOutputI2S           i2s1;           //xy=1092,315
AudioConnection          patchCord1(playMem6, fade6);
AudioConnection          patchCord2(playMem7, fade7);
AudioConnection          patchCord3(playMem1, fade1);
AudioConnection          patchCord4(playMem4, fade4);
AudioConnection          patchCord5(playMem2, fade2);
AudioConnection          patchCord6(playMem5, fade5);
AudioConnection          patchCord7(playMem8, fade8);
AudioConnection          patchCord8(playMem3, fade3);
AudioConnection          patchCord9(fade1, 0, mixer1, 0);
AudioConnection          patchCord10(fade2, 0, mixer1, 1);
AudioConnection          patchCord11(fade3, 0, mixer1, 2);
AudioConnection          patchCord12(fade4, 0, mixer1, 3);
AudioConnection          patchCord13(fade7, 0, mixer2, 2);
AudioConnection          patchCord14(fade6, 0, mixer2, 1);
AudioConnection          patchCord15(fade5, 0, mixer2, 0);
AudioConnection          patchCord16(fade8, 0, mixer2, 3);
AudioConnection          patchCord17(mixer1, 0, mixer3, 0);
AudioConnection          patchCord18(mixer2, 0, mixer3, 1);
AudioConnection          patchCord19(mixer3, 0, i2s1, 0);
AudioConnection          patchCord20(mixer3, 0, i2s1, 1);
AudioConnection          patchCord21(mixer3, queue1);
AudioControlSGTL5000     sgtl5000_1;     //xy=815,675
// GUItool: end automatically generated code

byte numDigits = 1;
byte digitPins[] = {};
byte segmentPins[] = {35, 36, 37, 38, 39, 33, 34};
bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
byte hardwareConfig = COMMON_CATHODE; // See README.md for options
bool updateWithDelays = false; // Default 'false' is Recommended
bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
bool disableDecPoint = true; // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]

const int LOWC = 0;
const int D = 1;
const int E = 2;
const int F = 3;
const int G = 4;
const int A = 5;
const int B = 6;
const int C = 7;
const int AnalogPin = 14;
ResponsiveAnalogRead analog(AnalogPin, true);

const int loader = 11;

const int leftMotor = 5;
const int rightMotor = 6;

Toggle lowCSwitch(24);
Toggle DSwitch(D + 24);
Toggle ESwitch(E + 24);
Toggle FSwitch(F + 24);
Toggle GSwitch(G + 24);
Toggle ASwitch(A + 24);
Toggle BSwitch(B + 24);
Toggle CSwitch(C + 24);
Toggle LoaderSwitch(loader);

const int TOTAL_SWITCHES = 8;
Toggle* ALL_SWITCHES[] = {&lowCSwitch, &DSwitch, &ESwitch, &FSwitch, &GSwitch, &ASwitch, &BSwitch, &CSwitch};
AudioPlayMemory* mems[] = {&playMem1, &playMem2, &playMem3, &playMem4, &playMem5, &playMem6, &playMem7, &playMem8};
AudioEffectFade* fades[] = {&fade1, &fade2, &fade3, &fade4, &fade5, &fade6, &fade7, &fade8};
const unsigned int* samples[] = {AudioSampleC1, AudioSampleD1, AudioSampleE1, AudioSampleF1, AudioSampleG1, AudioSampleA1, AudioSampleB1, AudioSampleC2};

const long COOLDOWN = 10;

unsigned long lastTreat = -1;

unsigned long motorStart = 0;

bool dispense = false;

int currSteps = 0;

int currLevel = 1;

int lastNote = -1;

int recording = false;

unsigned long lastNoteTime = 0;

File currRecording;

// use information from https://cran.r-project.org/web/packages/ctypesio/vignettes/wave-format.html
void writeWavHeader() {
  uint32_t finalSize = currRecording.size();

  currRecording.seek(0);
  byte header[44];

  memcpy(&header[0], "RIFF", 4);
  *(uint32_t*) (header + 4) = (finalSize - 8);
  memcpy(&header[8], "WAVE", 4);
  memcpy(&header[12], "fmt ", 4);
  *(uint32_t*) (header + 16) = 16;
  *(uint32_t*) (header + 20) = 1;
  *(uint32_t*) (header + 22) = 1;
  *(uint32_t*) (header + 24) = 44100;
  *(uint32_t*) (header + 28) = (44100*2);
  *(uint32_t*) (header + 32) = 2;
  *(uint32_t*) (header + 34) = 16;
  memcpy(&header[36], "data", 4);
  *(uint32_t*) (header + 40) = finalSize - 44;

  currRecording.write(header, 44);
}

const byte numbers[10][7] = {
  {1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 0, 0, 0, 0},
  {1, 1, 0, 1, 1, 0, 1},
  {1, 1, 1, 1, 0, 0, 1},
  {0, 1, 1, 0, 0, 1, 1},
  {1, 0, 1, 1, 0, 1, 1},
  {1, 0, 1, 1, 1, 1, 1},
  {1, 1, 1, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 0, 1, 1}
};

void displayNumber(int n) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], numbers[n][i]);
  }
}

#endif