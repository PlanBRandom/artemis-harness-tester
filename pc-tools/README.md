# PC Tools – Artemis Harness Tester

Python tools for the Artemis Harness Tester, including the CLI, CSV viewer, and live dashboard.

## Installation

```bash
pip install -r requirements.txt
```

## Live Dashboard (NEW! ⚡)

**Real-time GUI with matrix visualization and test controls**

```bash
python live_dashboard.py
```

**Features:**
- 🔴 Live serial connection to Artemis ATP
- 📊 Real-time 21×21 connectivity matrix heatmap
- 📈 Per-channel status and resistance display
- 🎮 One-click test controls (MEASURE, MATRIX, SELFTEST, PROFILE)
- 💾 Auto-capture of JSON responses
- 🖥️ Serial console for debugging

**Usage:**
1. Connect Artemis ATP via USB
2. Run `python live_dashboard.py`
3. Select COM port and click "Connect"
4. Click test buttons or watch auto-scrolling results

---

## CLI Commands

### Basic Commands

**Measure all channels:**
```bash
python tester_cli.py --port COM5 measure
```

**Run crosstalk matrix:**
```bash
python tester_cli.py --port COM5 matrix
```

**View current profile:**
```bash
python tester_cli.py --port COM5 profile
```

**Set channel profile:**
```bash
python tester_cli.py --port COM5 profile-set 3 1 0.0 2.0
```
- Channel 3
- Should be connected (1)
- Min: 0.0Ω
- Max: 2.0Ω

**Run self-test:**
```bash
python tester_cli.py --port COM5 selftest
```

**Run calibration:**
```bash
python tester_cli.py --port COM5 calibrate
```

### Port Selection

**Windows:**
```bash
python tester_cli.py --port COM5 measure
```

**Linux:**
```bash
python tester_cli.py --port /dev/ttyUSB0 measure
```

**macOS:**
```bash
python tester_cli.py --port /dev/cu.usbserial-* measure
```

## Output Formats

The CLI parses both human-readable and JSON output from the tester, displaying:
- Per-channel resistance values
- Pass/fail status
- Out-of-spec warnings
- Crosstalk matrix visualization

## Advanced Usage

### Custom Baud Rate
```bash
python tester_cli.py --port COM5 --baud 9600 measure
```

### Scripting Example

```bash
#!/bin/bash
# Automated test sequence
python tester_cli.py --port /dev/ttyUSB0 selftest
python tester_cli.py --port /dev/ttyUSB0 calibrate
python tester_cli.py --port /dev/ttyUSB0 measure > results.txt
python tester_cli.py --port /dev/ttyUSB0 matrix > matrix.txt
```

## Troubleshooting

**Permission denied on Linux:**
```bash
sudo usermod -a -G dialout $USER
# Log out and back in
```

**Port not found:**
```bash
# List available ports
python -m serial.tools.list_ports
```

**Timeout issues:**
Increase timeout in the code or check serial connection stability.
