#include <Arduino.h>
#include "commands.h"
#include "config.h"
#include "resistance.h"
#include "profiles.h"
#include "output.h"
#include "matrix.h"
#include "selftest.h"

static String read_line() {
    String s;
    while (true) {
        while (!Serial.available()) { /* idle */ }
        char c = Serial.read();
        if (c == '\n' || c == '\r') {
            if (s.length() > 0) return s;
        } else {
            s += c;
        }
    }
}

static void handle_command(const String& cmd) {
    if (cmd == "MEASURE") {
        Serial.println(F("=== CHANNEL SWEEP (HUMAN) ==="));
        for (int ch = 0; ch < NUM_CHANNELS; ++ch) {
            float r = resistance_measure_channel(ch);
            output_print_channel_human(ch, g_profile[ch], r);
        }

        Serial.println(F("=== CHANNEL SWEEP (JSON) ==="));
        Serial.println(F("{"));
        for (int ch = 0; ch < NUM_CHANNELS; ++ch) {
            float r = resistance_measure_channel(ch);
            bool last = (ch == NUM_CHANNELS - 1);
            output_print_channel_json(ch, g_profile[ch], r, last);
        }
        Serial.println(F("}"));

    } else if (cmd == "SELFTEST") {
        selftest_run();

    } else if (cmd == "MATRIX") {
        matrix_measure_crosstalk_json();

    } else if (cmd == "PROFILE") {
        Serial.println(F("{"));
        for (int ch = 0; ch < NUM_CHANNELS; ++ch) {
            bool last = (ch == NUM_CHANNELS - 1);
            output_print_profile_entry_json(ch, g_profile[ch], last);
        }
        Serial.println(F("}"));

    } else if (cmd.startsWith("PROFILE_SET")) {
        // format: PROFILE_SET ch should min max
        // example: PROFILE_SET 3 1 0.0 5.0
        int ch; int should; float minv, maxv;
        if (sscanf(cmd.c_str(), "PROFILE_SET %d %d %f %f", &ch, &should, &minv, &maxv) == 4) {
            bool ok = profiles_set_channel(ch, should != 0, minv, maxv);
            Serial.println(ok ? F("PROFILE_SET OK") : F("PROFILE_SET ERR"));
        } else {
            Serial.println(F("PROFILE_SET usage: PROFILE_SET ch should(0/1) min max"));
        }

    } else if (cmd == "CALIBRATE") {
        // simple: just rerun selftest for now
        bool ok = selftest_run();
        Serial.println(ok ? F("CALIBRATE OK") : F("CALIBRATE FAIL"));

    } else {
        Serial.print(F("Unknown command: "));
        Serial.println(cmd);
        Serial.println(F("Commands: MEASURE, MATRIX, SELFTEST, PROFILE, PROFILE_SET, CALIBRATE"));
    }
}

void commands_init() {
    Serial.println(F("Ready. Commands: MEASURE, MATRIX, SELFTEST, PROFILE, PROFILE_SET, CALIBRATE"));
}

void commands_poll() {
    if (Serial.available()) {
        String cmd = read_line();
        cmd.trim();
        if (cmd.length() > 0) {
            handle_command(cmd);
        }
    }
}
