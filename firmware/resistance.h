#pragma once
#include "config.h"

void  resistance_init();
float resistance_measure_raw();              // via CH0 / RES_NODE
float resistance_measure_channel(int ch);    // any CHx

bool  resistance_is_open(float r);
bool  resistance_is_short(float r);
bool  resistance_is_high_resistance(float r);

