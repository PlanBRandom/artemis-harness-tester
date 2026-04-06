#include <Arduino.h>
#include "resistance.h"
#include "adc_utils.h"
#include "config.h"

void resistance_init() {
    pinMode(PIN_RES_DRIVE, OUTPUT);
    digitalWrite(PIN_RES_DRIVE, LOW);
}

float resistance_measure_raw() {
    digitalWrite(PIN_RES_DRIVE, HIGH);
    delayMicroseconds(50);

    int adc = adc_read_oversampled(PIN_ADC_SENSE);
    digitalWrite(PIN_RES_DRIVE, LOW);

    float v_meas = adc_to_voltage(adc);
    if (v_meas <= 0.01f || v_meas >= (VDRIVE - 0.01f))
        return INFINITY;

    return RREF_OHMS * (v_meas / (VDRIVE - v_meas));
}

float resistance_measure_channel(int ch) {
    if (ch < 0 || ch >= NUM_CHANNELS) return INFINITY;

    if (ch == 0) {
        return resistance_measure_raw();
    }

    // For other channels: drive CHx HIGH, CH0 LOW, measure divider
    pinMode(CH_PIN[ch], OUTPUT);
    digitalWrite(CH_PIN[ch], HIGH);

    pinMode(CH_PIN[0], OUTPUT);
    digitalWrite(CH_PIN[0], LOW);

    float r = resistance_measure_raw();

    pinMode(CH_PIN[ch], INPUT);
    pinMode(CH_PIN[0], INPUT);

    return r;
}

bool resistance_is_open(float r) {
    return r >= OPEN_THRESHOLD_OHMS || !isfinite(r);
}

bool resistance_is_short(float r) {
    return r > 0 && r <= SHORT_THRESHOLD_OHMS;
}

bool resistance_is_high_resistance(float r) {
    return r > HIGH_RES_MIN_OHMS && r < OPEN_THRESHOLD_OHMS;
}
