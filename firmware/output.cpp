#include <Arduino.h>
#include "output.h"
#include "resistance.h"
#include "matrix.h"
#include "config.h"
#include "colors.h"

void output_print_channel_human(int ch, const ChannelSpec& spec, float r) {
    Serial.print(F("CH"));
    Serial.print(ch);
    Serial.print(F(": "));

    if (resistance_is_open(r)) {
        Serial.println(F("OPEN"));
    } else if (resistance_is_short(r)) {
        Serial.print(F("SHORT ("));
        Serial.print(r);
        Serial.println(F(" ohms)"));
    } else {
        Serial.print(r);
        Serial.print(F(" ohms"));
        bool out_of_spec = (r < spec.expected_min_ohms || r > spec.expected_max_ohms);
        if (out_of_spec) {
            Serial.print(F("  [OUT OF SPEC]"));
        }
        Serial.println();
    }
}

void output_print_channel_json(int ch, const ChannelSpec& spec, float r, bool last) {
    Serial.print(F("  \"CH"));
    Serial.print(ch);
    Serial.print(F("\": { \"r_ohms\": "));
    if (!isfinite(r)) Serial.print(F("null"));
    else Serial.print(r);

    Serial.print(F(", \"expected_min\": "));
    Serial.print(spec.expected_min_ohms);
    Serial.print(F(", \"expected_max\": "));
    Serial.print(spec.expected_max_ohms);
    Serial.print(F(", \"status\": \""));

    if (resistance_is_open(r)) Serial.print(F("OPEN"));
    else if (resistance_is_short(r)) Serial.print(F("SHORT"));
    else if (r < spec.expected_min_ohms || r > spec.expected_max_ohms) Serial.print(F("OUT_OF_SPEC"));
    else Serial.print(F("OK"));

    Serial.print(F("\" }"));
    if (!last) Serial.println(F(","));
    else Serial.println();
}

void output_print_profile_entry_json(int ch, const ChannelSpec& spec, bool last) {
    Serial.print(F("  \"CH"));
    Serial.print(ch);
    Serial.print(F("\": { \"should_be_connected\": "));
    Serial.print(spec.should_be_connected ? F("true") : F("false"));
    Serial.print(F(", \"expected_min\": "));
    Serial.print(spec.expected_min_ohms);
    Serial.print(F(", \"expected_max\": "));
    Serial.print(spec.expected_max_ohms);
    Serial.print(F(" }"));
    if (!last) Serial.println(F(","));
    else Serial.println();
}

void print_summary() {
    Serial.println(C_WHITE "\n=== SUMMARY ===" C_RESET);
    Serial.println(F("Test complete. Check CSV and JSON output above."));
}

void print_csv_matrix() {
    Serial.println(F("\nCSV_MATRIX_BEGIN"));
    // Header row
    Serial.print(F("CH"));
    for (int j = 0; j < NUM_CHANNELS; j++) {
        Serial.print(F(","));
        Serial.print(j);
    }
    Serial.println();
    
    // Data rows
    for (int i = 0; i < NUM_CHANNELS; i++) {
        Serial.print(i);
        for (int j = 0; j < NUM_CHANNELS; j++) {
            Serial.print(F(","));
            Serial.print(actual[i][j] ? 1 : 0);
        }
        Serial.println();
    }
    Serial.println(F("CSV_MATRIX_END"));
}

void print_json_matrix() {
    Serial.println(F("\nJSON_MATRIX_BEGIN"));
    Serial.println(F("{"));
    Serial.println(F("  \"matrix_type\": \"connectivity\","));
    Serial.println(F("  \"rows\": "));
    Serial.print(NUM_CHANNELS);
    Serial.println(F(","));
    Serial.println(F("  \"cols\": "));
    Serial.print(NUM_CHANNELS);
    Serial.println(F(","));
    Serial.println(F("  \"data\": ["));
    
    for (int i = 0; i < NUM_CHANNELS; i++) {
        Serial.print(F("    ["));
        for (int j = 0; j < NUM_CHANNELS; j++) {
            Serial.print(actual[i][j] ? 1 : 0);
            if (j < NUM_CHANNELS - 1) Serial.print(F(", "));
        }
        Serial.print(F("]"));
        if (i < NUM_CHANNELS - 1) Serial.println(F(","));
        else Serial.println();
    }
    
    Serial.println(F("  ]"));
    Serial.println(F("}"));
    Serial.println(F("JSON_MATRIX_END"));
}

