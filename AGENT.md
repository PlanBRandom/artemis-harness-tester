# 🤖 AI Coding Agent Guidelines

This document is **written for AI agents** that extend, maintain, or refactor code in this repository.

If you're a human developer, see README.md instead.

---

## Architecture at a Glance

This is a modular **harness connectivity tester** built on:

- **Firmware:** SparkFun Artemis ATP microcontroller
- **Core Logic:** 21-channel continuity + resistance + crosstalk scanner
- **Data Model:** Expected vs. Actual connectivity matrix
- **PC Tools:** Python serial reader + CSV exporter

The key insight: **Everything is derived from the netlist.** If you understand `nets.h` and `channels.h`, you understand the whole system.

---

## 🧬 Data Flow (Most Important)

```
channels.h (pin mapping)
    ↓
nets.h (net assignments)
    ↓
matrix.h (builds expected[21][21] from nets)
    ↓
matrix.h (measures actual[21][21] from hardware)
    ↓
compare_matrices() (flags opens, shorts, cross-wires)
    ↓
Serial output (CSV, JSON, summary)
    ↓
harness_viewer.py (capture & export)
```

**This is why the architecture is safe for agents to extend:**

- No magic numbers in the matrix logic
- All data transforms are explicit
- Adding a new test? Just add a function and call it from setup()

---

## 📋 Sacred Contracts (Do NOT Break)

These are non-negotiable. If an agent violates these, the firmware breaks.

### 1. The `channels[21]` Array
- **Never** change the order of channels without updating `net_id[21]` to match by index
- **Never** add/remove channels (firmware is hardcoded for 21)
- **OK to change:** Pin numbers, labels, Artemis pin assignments

### 2. The `net_id[21]` Array
- **One value per channel, in order** (index 0 = CH0, index 1 = CH1, etc.)
- **Each value is a net ID** (0–8 are pre-defined)
- **If you add a new net:** Create a comment, increment the next ID, document it

Example:
```cpp
// 0 = +3.3V
// 1 = -3.3V
// ...
// 9 = NEW_NET_NAME  ← You added this

int net_id[21] = {
    0, 1, 2, 3, 3,
    // ...
    9  ← Now this channel is on NEW_NET_NAME
};
```

### 3. The Matrix Dimensions
- **`expected[21][21]` and `actual[21][21]`** are **always 21×21**
- **Never** change dimensions without updating every loop in matrix.h
- **It's OK to add 2D matrices for other tests** (e.g., `resistance_map[21]`, `xtalk_count[21][21]`)

### 4. The Serial Output Markers
- **`CSV_MATRIX_BEGIN`** and **`CSV_MATRIX_END`** are **hardcoded in harness_viewer.py**
- **If you change these markers, the Python script breaks**
- If you need to add data, use new markers: `NEW_DATA_BEGIN`, `NEW_DATA_END`

---

## ✅ Safe Extensions for Agents

You can confidently add:

- **New tests:** `void my_custom_test() { ... }` called from setup()
- **New helper functions:** Resistance sub-routines, temperature checking, etc.
- **New output modes:** JSON variants, CSV headers, summary formats
- **New PC tools:** Python scripts to read CSV or JSON (don't modify serial protocol)
- **Hardware extension:** More GPIO, ADC channels, power measurement
- **Profiling:** Load different `net_id` arrays from EEPROM or SD card for different harnesses

Example safe addition:
```cpp
// In a new file: custom_tests.h
void temperature_check() {
    int temp = analogRead(TEMP_PIN);
    Serial.printf("TEMP: %d C\n", temp);
}

// Then call it from setup():
void setup() {
    // ... existing code ...
    temperature_check();  ← Agent added this
}
```

---

## ⚠️ Unsafe Changes (Agent Red Flags)

**An agent should STOP and ask before:**

- Changing `channels[21]` order or count
- Changing `net_id[21]` structure or size
- Modifying `expected[21][21]` logic (build_expected_matrix)
- Changing Serial output markers or CSV format
- Removing or renaming core test functions (compare_matrices, build_actual_matrix, etc.)

---

## 📁 File Responsibilities

### Firmware Files

| File | Responsibility | Agent Can Modify? |
|------|---|---|
| `channels.h` | Pin → name mapping | ✅ (keep order) |
| `nets.h` | Net assignments | ✅ (keep 21 values) |
| `matrix.h` | Core test logic | ⚠️ (extend, don't break) |
| `colors.h` | ANSI color codes | ✅ (freely) |
| `banner.h` | Startup banner | ✅ (freely) |
| `resistance.h` | Resistance measurement | ✅ (extend or refactor) |
| `xtalk.h` | Crosstalk detection | ✅ (extend or refactor) |
| `harness_tester.ino` | Main entry point | ⚠️ (add calls, don't remove) |

### PC Tools

| File | Responsibility | Agent Can Modify? |
|------|---|---|
| `harness_viewer.py` | CSV capture & export | ✅ (extend features, keep markers) |
| `harness_gui.py` | GUI visualization | ✅ (create if needed) |
| `harness_web.py` | Web dashboard | ✅ (create if needed) |

---

## 🎯 Common Agent Tasks

### Task: Add a new test (e.g., "leakage current")

1. Create a new file: `firmware/leakage.h`
2. Define: `void leakage_test() { ... }`
3. Include in `.ino`: `#include "leakage.h"`
4. Call from setup(): `leakage_test();`
5. ✅ Safe: Doesn't touch matrix or channels

### Task: Add GUI visualization

1. Create: `pc-tools/harness_gui.py`
2. Use existing CSV export from viewer.py
3. Plot matrix as heatmap, highlight fails
4. ✅ Safe: Reads from serial, doesn't modify protocol

### Task: Load different harness profiles

1. Create: `firmware/profiles.h`
2. Define `Profile` struct with name + net_id[] array
3. EEPROM/SD loader to swap profiles
4. ✅ Safe: Extends channels/nets, doesn't break them

### Task: Add resistance per-wire logging

1. Create: `firmware/resistance_log.h`
2. Measure resistance for each channel pair
3. Export as JSON or CSV (new marker: `RESISTANCE_BEGIN`, `RESISTANCE_END`)
4. Update Python viewer to capture this block
5. ✅ Safe: Adds new data, doesn't change existing markers

---

## 🧪 Testing Guidelines for Agents

Before committing changes:

1. **Firmware compile check:**
   - `#include` all headers in the .ino file
   - No redefinition warnings
   - Check that `net_id[21]` has exactly 21 values

2. **Logic check:**
   - `expected[21][21]` still auto-builds from `net_id[21]`
   - `compare_matrices()` still flags opens/shorts correctly
   - No serial output markers broken

3. **PC tool check:**
   - Python script still parses `CSV_MATRIX_BEGIN/END`
   - CSV export still produces valid 21×21 matrix
   - No crashes on malformed serial input

---

## 🚀 Recommended First Tasks for Agents

If you're being asked to extend this repo, here are safe starting points:

1. **Add a GUI** (harness_gui.py)
   - Read CSV output → visualize matrix as heatmap
   - Highlight pass/fail
   - Export PDF report

2. **Add a web dashboard** (harness_web.py)
   - Flask/FastAPI server
   - Real-time serial streaming
   - Historical test logs

3. **Add test profiles**
   - Create profiles/ JSON files with different harnesses
   - Firmware loader to swap net_id arrays
   - PC tool to select profile before test

4. **Add calibration routine**
   - Measure reference resistor value
   - Store in EEPROM
   - Use for accurate resistance calculation

5. **Add batch testing**
   - Thread/queue multiple test harnesses
   - Log results to database
   - Generate daily reports

---

## 📖 How to Ask Questions

If an agent encounters ambiguity:

- **Not covered in this doc?** → Check README.md
- **Architecture change?** → Stop and explain your plan in plain English
- **Unsure if safe?** → Default to "don't break the matrix" and "don't touch channels/nets"

---

## Summary for Agents

✅ **You can:**
- Add tests, helpers, output formats
- Extend PC tools with GUI/web
- Refactor code (as long as behavior is preserved)
- Create new modules for new features

⚠️ **You should ask about:**
- Changing channels[21] order or size
- Changing net_id[21] structure
- Modifying core test logic
- Changing Serial output format

🚫 **You MUST NOT:**
- Break the expected[21][21] ← actual[21][21] comparison
- Remove CSV_MATRIX_BEGIN/END markers
- Hardcode netlist instead of deriving from nets.h
- Assume anything about Artemis pins (they're in channels.h)

---

**Last Updated:** February 2026  
**Architecture Version:** 1.0  
**Firmware Platform:** SparkFun Artemis ATP
