#include "channels.h"
#include "nets.h"
#include "matrix.h"
#include "resistance.h"
#include "xtalk.h"
#include "output.h"
#include "colors.h"
#include "banner.h"

void setup() {
    Serial.begin(115200);
    delay(300);

    banner();

    Serial.println(C_WHITE "Building expected matrix…" C_RESET);
    build_expected_matrix();

    Serial.println(C_WHITE "Scanning harness…" C_RESET);
    build_actual_matrix();

    compare_matrices();

    // Crosstalk on SPI lines
    crosstalk_test(14); // SCK
    crosstalk_test(10); // SDI
    crosstalk_test(11); // SDO

    print_summary();

    print_csv_matrix();
    print_json_matrix();
}

void loop() {}
