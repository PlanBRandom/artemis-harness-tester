# Artemis Harness Tester

Production-grade 21-channel cable harness tester for SparkFun Artemis ATP fixtures.

## Features

- Auto-generated 21x21 connectivity matrix from a simple netlist
- Open, short, and crosstalk detection
- Per-channel resistance measurement with JSON output
- Profile-based limits for automated pass/fail checks
- Python CLI, live dashboard, and CSV viewer tools

## Quick Start

### Firmware

1. Open `firmware/harness_tester.ino` in Arduino IDE, or build from `platformio.ini`.
2. Install SparkFun Artemis board support.
3. Flash the firmware to the Artemis ATP.
4. Open a serial terminal at `115200` baud.

### PC tools

```bash
cd pc-tools
pip install -r requirements.txt
python tester_cli.py --port COM5 selftest
```

## Repository Layout

```text
firmware/      Firmware modules and entry points
pc-tools/      Python tools for CLI, dashboard, and CSV viewing
hardware/      KiCad design files, diagrams, and notes
profiles/      Example profile data
docs/          Assembly and validation notes
tests/         Basic test coverage
```

## Core Model

The tester is built around two source files:

- `firmware/channels.h`: channel index to physical pin mapping
- `firmware/nets.h`: channel index to expected net assignment

The firmware derives the expected matrix from `nets.h`, measures the actual matrix from hardware, then compares both to flag opens, shorts, and unexpected couplings.

## PC Tools

- `pc-tools/tester_cli.py`: command-line access to `MEASURE`, `MATRIX`, `PROFILE`, `SELFTEST`, and `CALIBRATE`
- `pc-tools/live_dashboard.py`: live serial console with matrix visualization
- `pc-tools/harness_viewer.py`: capture CSV matrix output from the serial stream
- `pc-tools/harness_gui.py`: simple CSV viewer
- `pc-tools/harness_web.py`: lightweight upload-and-view web UI

## Hardware Assets

- KiCad schematic: `hardware/kicad/artemis_harness_tester.kicad_sch`
- KiCad PCB: `hardware/kicad/artemis_harness_tester.kicad_pcb`
- Wiring diagram: `hardware/diagrams/ascii_wiring_diagram.txt`
- Validation notes: `WIRECHECK.md`

## Development Notes

- `AGENT.md` describes the safe extension boundaries for firmware and tools.
- `ARCHITECTURE.md` explains the modular firmware structure and command protocol.
- `QUICKSTART.md` covers bring-up, serial commands, and troubleshooting.

## License

MIT. See `LICENSE`.

MIT License (see LICENSE file)

---

## 🤖 AI Coding Agent Notes

This repo is structured to be AI-friendly:
- Modular headers
- Clear separation of concerns
- Predictable naming
- Explicit task list (see TASKS.md)
- Roadmap for future expansion (see ROADMAP.md)

Agents may safely:
- Add new test modules
- Extend PC tools
- Add GUIs or dashboards
- Add calibration routines
- Add new harness profiles

Agents should NOT:
- Modify `channels.h` or `nets.h` without explicit instruction
- Break the matrix API
- Remove CSV/JSON markers

---

## 📄 License

MIT License (see LICENSE file)

---

## 🤖 AI Coding Agent Notes

This repo is structured to be AI-friendly:
- Modular headers
- Clear separation of concerns
- Predictable naming
- Explicit task list (see TASKS.md)
- Roadmap for future expansion (see ROADMAP.md)

Agents may safely:
- Add new test modules
- Extend PC tools
- Add GUIs or dashboards
- Add calibration routines
- Add new harness profiles

Agents should NOT:
- Modify `channels.h` or `nets.h` without explicit instruction
- Break the matrix API
- Remove CSV/JSON markers
