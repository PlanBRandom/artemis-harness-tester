#pragma once
#include "colors.h"

void banner() {
    Serial.println(C_CYAN);
    Serial.println("========================================");
    Serial.println("      ARTEMIS HARNESS TESTER v1.0       ");
    Serial.println("========================================");
    Serial.println(C_RESET);
}
