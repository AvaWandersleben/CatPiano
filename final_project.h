#ifndef FINALPROJECT
#define FINALPROJECT

#include <Toggle.h>
#include <Stepper.h>

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
AudioPlayMemory          playMem6;       //xy=142,555
AudioPlayMemory          playMem7;       //xy=147,618
AudioPlayMemory          playMem1;       //xy=161,140
AudioPlayMemory          playMem4;       //xy=162,392
AudioPlayMemory          playMem2;       //xy=169,214
AudioPlayMemory          playMem5;       //xy=174,474
AudioPlayMemory          playMem8;       //xy=180,682
AudioPlayMemory          playMem3;       //xy=186,309
AudioEffectFade          fade1;          //xy=308,145
AudioEffectFade          fade2;          //xy=345,207
AudioEffectFade          fade3;          //xy=361,300
AudioEffectFade          fade4;          //xy=392,387
AudioEffectFade          fade7;          //xy=393,625
AudioEffectFade          fade6;          //xy=406,558
AudioEffectFade          fade5;          //xy=414,464
AudioEffectFade          fade8;          //xy=417,679
AudioMixer4              mixer1;         //xy=621,245
AudioMixer4              mixer2;         //xy=620,547
AudioMixer4              mixer3;         //xy=801,387
AudioOutputI2S           i2s1;           //xy=1118,412
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
AudioControlSGTL5000     sgtl5000_1;     //xy=841,772
// GUItool: end automatically generated code


const int STEPS = 2048;

const int LOWC = 0;
const int D = 1;
const int E = 2;
const int F = 3;
const int G = 4;
const int A = 5;
const int B = 6;
const int C = 7;

const int loader = 11;

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

Stepper stepper(STEPS, 3, 5, 4, 6);

const long COOLDOWN = 10;

long lastTreat = -1;

bool dispense = false;

bool loading = false;

int currSteps = 0;

#endif