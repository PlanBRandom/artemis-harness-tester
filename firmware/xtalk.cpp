#include <Arduino.h>
#include "xtalk.h"
#include "channels.h"
#include "colors.h"

// Global crosstalk flag
bool any_xTalk = false;

void crosstalk_test(int aggressor) {
    Serial.printf(C_MAGENTA "\nXTALK TEST: %s\n" C_RESET, channels[aggressor].name);

    pinMode(channels[aggressor].pin, OUTPUT);

    for (int t = 0; t < 2000; t++) {
        digitalWrite(channels[aggressor].pin, HIGH);
        delayMicroseconds(2);
        digitalWrite(channels[aggressor].pin, LOW);
        delayMicroseconds(2);

        for (int j = 0; j < 21; j++) {
            if (j == aggressor) continue;

            if (digitalRead(channels[j].pin) == HIGH) {
                any_xTalk = true;
                Serial.printf(C_YELLOW "XTALK: %s → %s\n" C_RESET,
                    channels[aggressor].name, channels[j].name);
            }
        }
    }

    pinMode(channels[aggressor].pin, INPUT);
}
