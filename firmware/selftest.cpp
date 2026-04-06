#include <Arduino.h>
#include "selftest.h"
#include "resistance.h"
#include "config.h"

bool selftest_run() {
    Serial.println(F("SELFTEST: Measuring RREF path via CH0..."));
    float r = resistance_measure_raw();
    Serial.print(F("SELFTEST: Measured RREF path = "));
    Serial.print(r);
    Serial.println(F(" ohms"));

    if (!isfinite(r)) {
        Serial.println(F("SELFTEST: FAIL (non-finite reading)"));
        return false;
    }

    if (r < RREF_OHMS * 0.5f || r > RREF_OHMS * 1.5f) {
        Serial.println(F("SELFTEST: FAIL (out of expected range)"));
        return false;
    }

    Serial.println(F("SELFTEST: PASS"));
    return true;
}
