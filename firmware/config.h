#pragma once
#include <Arduino.h>

constexpr int NUM_CHANNELS     = 21;

constexpr int PIN_RES_DRIVE    = 23;   // RES_DRIVE
constexpr int PIN_ADC_SENSE    = 39;   // RES_NODE sense (Apollo3 AD0)

constexpr float VDRIVE         = 3.3f;
constexpr float RREF_OHMS      = 1000.0f;

constexpr int   ADC_MAX        = 1023;
constexpr int   ADC_OVERSAMPLE = 16;

// Thresholds
constexpr float OPEN_THRESHOLD_OHMS   = 50000.0f;
constexpr float SHORT_THRESHOLD_OHMS  = 10.0f;
constexpr float HIGH_RES_MIN_OHMS     = 200.0f;

// Channel → pin map
extern const int CH_PIN[NUM_CHANNELS];
