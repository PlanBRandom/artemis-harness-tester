# Artemis Harness Tester – Firmware

Instrument‑grade harness tester firmware for SparkFun Artemis ATP + 21‑channel resistor‑isolated matrix.

## Overview

- 21 channels (CH0–CH20), each with 1 kΩ series resistor
- Dedicated resistance engine using RREF and RES_NODE
- Self‑test, per‑channel resistance, crosstalk matrix
- Human‑readable and JSON output over serial
- Simple command protocol for PC tools

## Serial Command Protocol

**Baud:** `115200`

Send one of the following lines (terminated with `\n`):

### `MEASURE`

Per‑channel resistance sweep.

**Human section:**
```
=== CHANNEL SWEEP (HUMAN) ===
CH0: 0.8 ohms
CH1: OPEN
CH2: SHORT (0.5 ohms)
...
```

**JSON section:**
```json
{
  "CH0": { "r_ohms": 0.8, "expected_min": 0.0, "expected_max": 5.0, "status": "OK" },
  "CH1": { "r_ohms": null, "expected_min": 0.0, "expected_max": 5.0, "status": "OPEN" },
  "CH2": { "r_ohms": 0.5, "expected_min": 0.0, "expected_max": 5.0, "status": "SHORT" }
}
```

### `SELFTEST`

Runs internal self‑test of the RREF path.

- Checks measured RREF path against RREF_OHMS
- Prints PASS/FAIL and measured value

### `MATRIX`

Measures a simple crosstalk/leakage matrix between channels.

**Output (JSON):**
```json
{
  "matrix": [
    [0.0, 12345.0, null, ...],
    [12000.0, 0.0, 8000.0, ...],
    ...
  ]
}
```

- `matrix[i][j]` is the apparent resistance between CHi and CHj
- `0.0` on diagonal
- `null` if measurement saturated / invalid

### `PROFILE`

Dumps the current expected profile:

```json
{
  "CH0": { "should_be_connected": true, "expected_min": 0.0, "expected_max": 5.0 },
  "CH1": { "should_be_connected": true, "expected_min": 0.0, "expected_max": 5.0 }
}
```

### `PROFILE_SET ch should min max`

Sets the expected profile for a specific channel.

**Example:**
```
PROFILE_SET 3 1 0.0 2.0
```
- `ch`: Channel number (0-20)
- `should`: Should be connected (0/1)
- `min`: Minimum expected resistance (ohms)
- `max`: Maximum expected resistance (ohms)

**Response:**
```
PROFILE_SET OK
```
or
```
PROFILE_SET ERR
```

### `CALIBRATE`

Runs calibration routine (currently alias for SELFTEST).

## Module Layout

| Module | Description |
|--------|-------------|
| `config.*` | Pins, constants, thresholds |
| `adc_utils.*` | Oversampled ADC + voltage conversion |
| `profiles.*` | Expected per‑channel specs |
| `resistance.*` | RREF engine + per‑channel resistance |
| `matrix.*` | Crosstalk matrix (JSON) |
| `output.*` | Human + JSON formatting |
| `selftest.*` | Self‑test of RREF path |
| `commands.*` | Serial command parser + dispatcher |
| `main.cpp` | Setup + main loop |

## Extending

- Add profile loading from flash or serial JSON
- Add more commands (MATRIX_FULL, PROFILE_LOAD, PROFILE_SAVE)
- Integrate with a PC‑side CLI or GUI that speaks this protocol
- Add timestamp and test metadata to JSON output
- Add batch testing with run IDs

## Building

This firmware is designed for Arduino IDE or PlatformIO with SparkFun Artemis ATP board support.

**Arduino IDE:**
1. Install SparkFun Apollo3 boards via Board Manager
2. Select "SparkFun Artemis ATP" from Tools → Board
3. Open `main.cpp` (or rename to `.ino`)
4. Upload

**PlatformIO:**
```ini
[env:sparkfun_artemis_atp]
platform = apollo3blue
board = sparkfun_artemis_atp
framework = arduino
```

## PC Tools

See `../pc-tools/tester_cli.py` for a Python CLI that speaks this protocol.

**Usage examples:**
```bash
python tester_cli.py --port /dev/ttyUSB0 measure
python tester_cli.py --port /dev/ttyUSB0 matrix
python tester_cli.py --port /dev/ttyUSB0 profile
python tester_cli.py --port /dev/ttyUSB0 profile-set 3 1 0.0 2.0
python tester_cli.py --port /dev/ttyUSB0 selftest
python tester_cli.py --port /dev/ttyUSB0 calibrate
```
