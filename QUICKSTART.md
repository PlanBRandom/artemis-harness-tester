# Artemis Harness Tester – Quick Start Guide

Production-grade harness continuity tester with resistance measurement.

## Hardware

- **Board:** SparkFun Artemis ATP
- **Channels:** 21 (CH0–CH20)
- **Series Resistors:** 1kΩ 0805 (RDRV0–RDRV20)
- **Reference Resistor:** 1kΩ 1% 0805 (RREF)
- **Connector:** 21-position terminal block (5.08mm pitch)
- **PCB:** 140mm × 70mm, 2-layer

## Firmware Setup

### 1. Install Arduino IDE
- Download from [arduino.cc](https://www.arduino.cc)
- Install SparkFun Apollo3 board support via Board Manager

### 2. Configure Arduino IDE
- Tools → Board → SparkFun Artemis ATP
- Tools → Port → Select COM port
- Tools → Upload Speed → 115200

### 3. Upload Firmware
- Open `firmware/main.cpp` (or rename to `.ino`)
- Click Upload button
- Monitor serial output at 115200 baud

### 4. Verify Operation
```
ARTEMIS HARNESS TESTER - PRO MODE
SELFTEST: Measuring RREF path via CH0...
SELFTEST: Measured RREF path = 1003.2 ohms
SELFTEST: PASS
Ready. Commands: MEASURE, MATRIX, SELFTEST, PROFILE, PROFILE_SET, CALIBRATE
```

## PC Tools Setup

### 1. Install Python Dependencies
```bash
cd pc-tools
pip install -r requirements.txt
```

### 2. Test Connection
```bash
python tester_cli.py --port COM5 selftest
```

### 3. Run Measurement
```bash
python tester_cli.py --port COM5 measure
```

## Usage

### Testing a Harness

1. **Connect harness** to J1 terminal block (21 positions)
2. **Run self-test:**
   ```bash
   python tester_cli.py --port COM5 selftest
   ```
3. **Measure all channels:**
   ```bash
   python tester_cli.py --port COM5 measure
   ```
4. **Check crosstalk matrix:**
   ```bash
   python tester_cli.py --port COM5 matrix
   ```

### Setting Test Profiles

Configure expected resistance ranges for each channel:

```bash
# Channel 0: expect 0.0–2.0Ω when connected
python tester_cli.py --port COM5 profile-set 0 1 0.0 2.0

# Channel 5: should be open (not connected)
python tester_cli.py --port COM5 profile-set 5 0 0.0 0.0
```

### Serial Terminal (Manual)

Open serial monitor at 115200 baud and send:
- `MEASURE` – Measure all channels
- `MATRIX` – Crosstalk matrix
- `PROFILE` – Show current profile
- `PROFILE_SET ch should min max` – Set channel spec
- `SELFTEST` – Run self-test
- `CALIBRATE` – Run calibration

## Interpreting Results

### Channel Status
- **OK:** Resistance within expected range
- **OPEN:** No connection detected (R > 50kΩ)
- **SHORT:** Very low resistance (R < 10Ω)
- **OUT_OF_SPEC:** Connected but outside expected range

### Typical Values
- **Wire jumper:** 0.5–2.0Ω
- **PCB trace short:** 0.1–0.5Ω
- **Connector resistance:** 10–50mΩ per contact
- **Open circuit:** null (infinite)

## Troubleshooting

### SELFTEST FAIL
- Check RREF soldering (should read ~1000Ω)
- Verify PIN23 and A0 connections
- Check for solder bridges

### All Channels OPEN
- Verify harness is connected to J1
- Check terminal block connections
- Run SELFTEST to verify hardware

### Erratic Readings
- Check power supply stability (3.3V)
- Add decoupling capacitors if not present
- Reduce ADC oversampling if needed

### High Noise
- Ensure good ground connection
- Keep harness away from switching power supplies
- Shield cables if testing in noisy environment

## Next Steps

- Review [firmware/FIRMWARE_README.md](firmware/FIRMWARE_README.md) for API details
- Review [pc-tools/README.md](pc-tools/README.md) for CLI options
- See [hardware/notes/kikad.txt](hardware/notes/kikad.txt) for circuit details
- Check [WIRECHECK.md](WIRECHECK.md) for hardware validation

## Support

For issues or questions, refer to the project documentation or check:
- Firmware source: `firmware/`
- Hardware design: `hardware/kicad/`
- Validation report: `WIRECHECK.md`
