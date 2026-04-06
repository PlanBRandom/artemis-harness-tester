#pragma once
#include "config.h"

int   adc_read_oversampled(int pin, int samples = ADC_OVERSAMPLE);
float adc_to_voltage(int adc);
