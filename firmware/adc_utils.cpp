#include <Arduino.h>
#include "adc_utils.h"

int adc_read_oversampled(int pin, int samples) {
    long acc = 0;
    for (int i = 0; i < samples; ++i) {
        acc += analogRead(pin);
    }
    return acc / samples;
}

float adc_to_voltage(int adc) {
    return (adc / float(ADC_MAX)) * VDRIVE;
}
