# Artemis Harness Tester

Production-grade cable harness tester using SparkFun Artemis ATP for P3/P1/P2 assemblies with automated continuity, resistance, leakage, and zener tests.

## 🚀 Quick Start

1. **Hardware Setup**
   - Flash `firmware/harness_tester.ino` to SparkFun Artemis ATP
   - Connect resistor arrays per schematic in `docs/harness_tester.md`
   - Insert harness into test jig

2. **Run Test**
   - Open serial monitor at 115200 baud
   - Read JSON report output

3. **Analyze Results**
   ```bash
   python tools/parse_report.py test_output.json
   ```

## 📁 Repository Structure

```
├── firmware/
│   └── harness_tester.ino          # Arduino firmware for Artemis ATP
├── docs/
│   └── harness_tester.md           # Complete documentation
├── test_definitions/
│   └── harness_test.json           # Test configuration
├── tools/
│   └── parse_report.py             # Python analysis tool
└── README.md                        # This file
```

## 🔧 Features

- **Continuity Testing**: Detects open circuits and broken conductors
- **Resistance Measurement**: Identifies weak solder joints (GOOD/MARGINAL/BAD)
- **Leakage Detection**: Finds flux contamination and solder whiskers
- **Zener Characterization**: Validates protection diode D3
- **JSON Reports**: Machine-readable test results

## 📊 Test Coverage

**P3 Connector (7-pin)**
- SDI, SDO, SCK, ADC_CS, POT_CS
- +3.3V_FUSED (with Zener test)
- GND

**P2 Connector (5-pin)**
- ADC_CS, SDI, POT_CS, SDO, SCK

**P1 Connector (5-pin)**
- +3.3V_FUSED, -3.3V, +ADJ
- GND (×2)

## 🛠️ Hardware Requirements

- SparkFun Artemis ATP
- Bourns resistor arrays:
  - CAY10-472J8LF (8× 4.7kΩ) × 3
  - CAY16-473J4LF (4× 47kΩ) × 1
- Custom PCB with P3/P2/P1 connectors
- 3D-printed test jig (optional)

## 📖 Documentation

See `docs/harness_tester.md` for complete documentation including:
- Pin mappings
- Firmware architecture
- Test methodology
- Hardware schematics
- Jig design notes
- Advanced engineering details

## 🤝 Contributing

This is a production tool for specific cable assemblies. Fork and adapt for your own harness testing needs!

## 📜 License

MIT License - See LICENSE file for details

---

**Author:** Brandon + GitHub Copilot  
**Status:** Production Ready 🟢