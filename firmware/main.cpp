#include <Arduino.h>
#include "config.h"
#include "profiles.h"
#include "resistance.h"
#include "selftest.h"
#include "commands.h"

void setup() {
    Serial.begin(115200);
    while (!Serial) { /* some boards */ }

    Serial.println(F("ARTEMIS HARNESS TESTER - PRO MODE"));

    profiles_init_default();
    resistance_init();
    selftest_run();
    commands_init();
}

void loop() {
    commands_poll();
}
