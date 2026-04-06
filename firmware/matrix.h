#pragma once
#include "config.h"
#include "profiles.h"

// 2D matrices for expected vs actual connectivity
extern bool expected[NUM_CHANNELS][NUM_CHANNELS];
extern bool actual[NUM_CHANNELS][NUM_CHANNELS];

void build_expected_matrix();
void build_actual_matrix();
void compare_matrices();
void matrix_measure_crosstalk_json();
