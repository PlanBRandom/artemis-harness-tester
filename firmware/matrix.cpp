#include <Arduino.h>
#include "matrix.h"
#include "resistance.h"
#include "config.h"
#include "channels.h"
#include "nets.h"
#include "colors.h"

// 2D matrices for expected vs actual connectivity
bool expected[NUM_CHANNELS][NUM_CHANNELS];
bool actual[NUM_CHANNELS][NUM_CHANNELS];

void build_expected_matrix() {
    // Two channels are connected if they're on the same net
    for (int i = 0; i < NUM_CHANNELS; i++) {
        for (int j = 0; j < NUM_CHANNELS; j++) {
            expected[i][j] = (net_id[i] == net_id[j]);
        }
    }
    Serial.println(C_GREEN "Expected matrix built." C_RESET);
}

void build_actual_matrix() {
    // Measure resistance between each pair of channels
    for (int i = 0; i < NUM_CHANNELS; i++) {
        for (int j = 0; j < NUM_CHANNELS; j++) {
            if (i == j) {
                actual[i][j] = true; // Same channel always "connected" to itself
                continue;
            }
            
            // Drive channel i, sense channel j
            pinMode(channels[i].pin, OUTPUT);
            digitalWrite(channels[i].pin, HIGH);
            delayMicroseconds(10);
            
            float r = resistance_measure_raw();
            // Connected if <1k ohm, open if very high
            actual[i][j] = (r < 1000.0f);
            
            pinMode(channels[i].pin, INPUT);
        }
    }
    Serial.println(C_GREEN "Actual matrix scanned." C_RESET);
}

void compare_matrices() {
    Serial.println(C_CYAN "=== COMPARISON ===" C_RESET);
    
    int opens = 0, shorts = 0, crosstalk_count = 0;
    
    for (int i = 0; i < NUM_CHANNELS; i++) {
        for (int j = 0; j < NUM_CHANNELS; j++) {
            if (i == j) continue;
            
            bool exp = expected[i][j];
            bool act = actual[i][j];
            
            if (exp && !act) {
                opens++;
                Serial.printf(C_RED "OPEN: %s ←→ %s\n" C_RESET, 
                    channels[i].name, channels[j].name);
            } else if (!exp && act) {
                crosstalk_count++;
                Serial.printf(C_YELLOW "CROSSTALK: %s ←→ %s\n" C_RESET,
                    channels[i].name, channels[j].name);
            }
        }
    }
    
    Serial.printf("\nOpens: %d, Crosstalk: %d\n", opens, crosstalk_count);
}

void matrix_measure_crosstalk_json() {
    Serial.println(F("{"));
    Serial.println(F("  \"matrix\": ["));

    for (int i = 0; i < NUM_CHANNELS; ++i) {
        Serial.print(F("    ["));
        for (int j = 0; j < NUM_CHANNELS; ++j) {
            float r = 0.0f;
            if (i == j) {
                r = 0.0f;
            } else {
                pinMode(CH_PIN[i], OUTPUT);
                digitalWrite(CH_PIN[i], HIGH);

                pinMode(CH_PIN[j], OUTPUT);
                digitalWrite(CH_PIN[j], LOW);

                r = resistance_measure_raw();

                pinMode(CH_PIN[i], INPUT);
                pinMode(CH_PIN[j], INPUT);
            }

            if (!isfinite(r)) Serial.print(F("null"));
            else Serial.print(r);

            if (j != NUM_CHANNELS - 1) Serial.print(F(", "));
        }
        Serial.print(F("]"));
        if (i != NUM_CHANNELS - 1) Serial.println(F(","));
        else Serial.println();
    }

    Serial.println(F("  ]"));
    Serial.println(F("}"));
}
