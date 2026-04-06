# Contributing to Artemis Harness Tester

Thank you for contributing! This project is designed to be AI-agent-friendly.

## Before You Start

Read AGENT.md first. It explains the data model, sacred contracts, and safe extensions.

## Development Setup

### Firmware
- Arduino IDE or PlatformIO
- SparkFun Artemis ATP board support

### PC Tools
- Python 3.8+
- `pip install pyserial`

## How to Contribute

### New Firmware Features
- New tests → create new `.h` file in firmware/
- Call from setup() in `.ino`
- Don't change channels.h or nets.h structure

### New PC Tools
- Create new `.py` file in pc-tools/
- Use existing CSV/JSON output
- Don't modify serial markers

### Code Style
- C++ for firmware, Python 3.8+ for PC tools
- `snake_case` for functions, `CAPS` for macros
- Comment complex logic
- No magic numbers

## Testing

1. Firmware compiles without errors
2. expected[21][21] still builds correctly
3. CSV_MATRIX_BEGIN/END markers intact
4. Python tools still parse output

## Questions?

See AGENT.md for detailed architecture guidance.
