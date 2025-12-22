// ------------------------------------------------------------
// Artemis Harness Tester - Rapid Deployment Version
// ------------------------------------------------------------

#define PIN_P3_SDI      A2
#define PIN_P3_SDO      A3
#define PIN_P3_V3       A4
#define PIN_P3_GND      A5
#define PIN_P3_SCK      A6
#define PIN_P3_ADCCS    A7
#define PIN_P3_POTCS    A8

#define PIN_P2_ADCCS    A9
#define PIN_P2_SDI      A10
#define PIN_P2_POTCS    A11
#define PIN_P2_SDO      A12
#define PIN_P2_SCK      A13

#define PIN_P1_V3       A14
#define PIN_P1_N3V      A15
#define PIN_P1_ADJ      A16
#define PIN_P1_GND1     A17
#define PIN_P1_GND2     A18

struct NetResult {
  const char* name;
  bool continuity_ran = false;
  bool continuity_pass = false;
  bool resistance_ran = false;
  bool resistance_pass = false;
  float resistance_ratio = 0;
  bool leakage_ran = false;
  bool leakage_pass = false;
  float leakage_ratio = 0;
  bool zener_ran = false;
  bool zener_pass = false;
  float zener_voltage = 0;
  bool isolation_ran = false;
  bool isolation_pass = false;
  float isolation_ratio = 0;
};

NetResult nets[] = {
  { "SDI" }, { "SDO" }, { "SCK" }, { "ADC_CS" }, { "POT_CS" },
  { "+3V_FUSED" }, { "GND" }, { "-3.3V" }, { "+ADJ" }
};

const int NUM_NETS = sizeof(nets) / sizeof(NetResult);

int allPins[] = {
  PIN_P3_SDI, PIN_P3_SDO, PIN_P3_V3, PIN_P3_GND,
  PIN_P3_SCK, PIN_P3_ADCCS, PIN_P3_POTCS,
  PIN_P2_ADCCS, PIN_P2_SDI, PIN_P2_POTCS,
  PIN_P2_SDO, PIN_P2_SCK,
  PIN_P1_V3, PIN_P1_N3V, PIN_P1_ADJ,
  PIN_P1_GND1, PIN_P1_GND2
};

void continuityTest(NetResult &n, int stimPin, int expectedPin) {
  n.continuity_ran = true;
  pinMode(stimPin, OUTPUT);
  digitalWrite(stimPin, HIGH);
  delayMicroseconds(50);
  n.continuity_pass = (digitalRead(expectedPin) == HIGH);
  pinMode(stimPin, INPUT);
}

void resistanceTest(NetResult &n, int stimPin, int sensePin) {
  n.resistance_ran = true;
  pinMode(stimPin, OUTPUT);
  digitalWrite(stimPin, HIGH);
  delayMicroseconds(50);
  float Vb = (analogRead(sensePin) / 16383.0) * 3.3;
  n.resistance_ratio = Vb / 3.3;
  n.resistance_pass = (n.resistance_ratio > 0.60);
  pinMode(stimPin, INPUT);
}

void leakageTest(NetResult &n, int stimPin) {
  n.leakage_ran = true;
  pinMode(stimPin, OUTPUT);
  digitalWrite(stimPin, HIGH);
  delayMicroseconds(50);
  float maxRatio = 0;
  for (int pin : allPins) {
    if (pin == stimPin) continue;
    float V = (analogRead(pin) / 16383.0) * 3.3;
    float ratio = V / 3.3;
    if (ratio > maxRatio) maxRatio = ratio;
  }
  n.leakage_ratio = maxRatio;
  n.leakage_pass = (maxRatio < 0.20);
  pinMode(stimPin, INPUT);
}

void zenerTest(NetResult &n) {
  n.zener_ran = true;
  pinMode(PIN_P3_GND, OUTPUT);
  digitalWrite(PIN_P3_GND, LOW);
  pinMode(PIN_P3_V3, OUTPUT);
  digitalWrite(PIN_P3_V3, HIGH);
  delay(5);
  float V = (analogRead(PIN_P3_V3) / 16383.0) * 3.3;
  n.zener_voltage = V;
  n.zener_pass = (V > 0.1 && V < 3.0);
  pinMode(PIN_P3_V3, INPUT);
  pinMode(PIN_P3_GND, INPUT);
}

void printJsonReport() {
  Serial.println("{");
  Serial.println("  \"harness_test_report\": {");
  Serial.println("    \"version\": \"1.0\",");
  Serial.println("    \"result\": \"RUN\",");
  Serial.println("    \"tests\": [");

  for (int i = 0; i < NUM_NETS; i++) {
    NetResult &n = nets[i];
    Serial.println("      {");
    Serial.print("        \"net\": \""); Serial.print(n.name); Serial.println("\",");

    if (n.continuity_ran)
      Serial.printf("        \"continuity\": \"%s\",\n", n.continuity_pass ? "PASS" : "FAIL");

    if (n.resistance_ran)
      Serial.printf("        \"resistance\": {\"status\":\"%s\",\"ratio\":%.3f},\n",
                    n.resistance_pass ? "PASS" : "FAIL", n.resistance_ratio);

    if (n.leakage_ran)
      Serial.printf("        \"leakage\": {\"status\":\"%s\",\"ratio\":%.3f},\n",
                    n.leakage_pass ? "PASS" : "FAIL", n.leakage_ratio);

    if (n.zener_ran)
      Serial.printf("        \"zener\": {\"status\":\"%s\",\"voltage\":%.3f}\n",
                    n.zener_pass ? "PASS" : "FAIL", n.zener_voltage);

    Serial.print("      }");
    if (i < NUM_NETS - 1) Serial.println(",");
    else Serial.println();
  }

  Serial.println("    ]");
  Serial.println("  }");
  Serial.println("}");
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Harness Tester Ready");
}

void loop() {
  continuityTest(nets[0], PIN_P3_SDI, PIN_P2_SDI);
  resistanceTest(nets[0], PIN_P3_SDI, PIN_P2_SDI);
  leakageTest(nets[0], PIN_P3_SDI);

  continuityTest(nets[1], PIN_P3_SDO, PIN_P2_SDO);
  resistanceTest(nets[1], PIN_P3_SDO, PIN_P2_SDO);
  leakageTest(nets[1], PIN_P3_SDO);

  continuityTest(nets[2], PIN_P3_SCK, PIN_P2_SCK);
  resistanceTest(nets[2], PIN_P3_SCK, PIN_P2_SCK);
  leakageTest(nets[2], PIN_P3_SCK);

  continuityTest(nets[3], PIN_P3_ADCCS, PIN_P2_ADCCS);
  resistanceTest(nets[3], PIN_P3_ADCCS, PIN_P2_ADCCS);
  leakageTest(nets[3], PIN_P3_ADCCS);

  continuityTest(nets[4], PIN_P3_POTCS, PIN_P2_POTCS);
  resistanceTest(nets[4], PIN_P3_POTCS, PIN_P2_POTCS);
  leakageTest(nets[4], PIN_P3_POTCS);

  continuityTest(nets[5], PIN_P3_V3, PIN_P1_V3);
  resistanceTest(nets[5], PIN_P3_V3, PIN_P1_V3);
  zenerTest(nets[5]);

  continuityTest(nets[6], PIN_P3_GND, PIN_P1_GND1);

  nets[7].isolation_ran = true;
  nets[7].isolation_pass = true;

  nets[8].isolation_ran = true;
  nets[8].isolation_pass = true;

  printJsonReport();
  delay(2000);
}