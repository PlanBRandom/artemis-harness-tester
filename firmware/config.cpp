#include "config.h"
#include "channels.h"
#include "nets.h"

const int CH_PIN[NUM_CHANNELS] = {
    2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22
};

// Global channel definitions
Channel channels[21] = {
    {2,  "P1-1 +3.3V"},
    {3,  "P1-2 -3.3V"},
    {4,  "P1-3 +ADJ"},
    {5,  "P1-4 GND"},
    {6,  "P1-5 GND"},

    {7,  "P2-1 ADC_CS"},
    {8,  "P2-2 MCU_SDI"},
    {9,  "P2-3 POT_CS"},
    {10, "P2-4 MCU_SDO"},
    {11, "P2-5 MCU_SCK"},

    {12, "P3-1 MCU_SDI"},
    {13, "P3-2 MCU_SDO"},
    {14, "P3-3 +3.3V"},
    {15, "P3-4 GND"},
    {16, "P3-5 MCU_SCK"},
    {17, "P3-6 ADC_CS"},
    {18, "P3-7 POT_CS"},

    {19, "HD1 GND"},
    {20, "HD2 GND"},
    {21, "TP1 GND"},
    {22, "TP2 GND"}
};

// Global net ID definitions
int net_id[21] = {
    0,1,2,3,3,
    7,4,8,5,6,
    4,5,0,3,6,
    7,8,3,3,3,3
};

