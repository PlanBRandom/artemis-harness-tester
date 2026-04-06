#!/usr/bin/env python3
"""
Artemis Harness Tester CLI

Command-line interface for the Artemis Harness Tester instrument.
Sends commands over serial and parses JSON responses.
"""
import argparse
import json
import sys
import time

import serial


def send_cmd(ser, cmd):
    """Send a command string to the tester."""
    ser.write((cmd + "\n").encode("ascii"))
    ser.flush()


def read_block(ser, timeout=2.0):
    """Read lines from serial until timeout or natural end."""
    ser.timeout = timeout
    lines = []
    while True:
        line = ser.readline()
        if not line:
            break
        s = line.decode("utf-8", errors="replace").rstrip()
        lines.append(s)
        # crude: stop when we see a closing brace for JSON or blank after header
        if s == "}" or s == "" and len(lines) > 0:
            break
    return lines


def cmd_measure(ser):
    """Run MEASURE command and display results."""
    send_cmd(ser, "MEASURE")
    lines = read_block(ser)
    # split human + JSON: last JSON block starts at first '{'
    try:
        idx = next(i for i, l in enumerate(lines) if l.strip().startswith("{"))
    except StopIteration:
        print("\n".join(lines))
        return
    human = lines[:idx]
    js = "\n".join(lines[idx:])
    print("\n".join(human))
    try:
        data = json.loads(js)
    except Exception:
        print(js)
        return

    print("\nParsed JSON:")
    for ch, info in data.items():
        print(f"{ch:>4}: {info['r_ohms']} Ω  status={info['status']}")


def cmd_matrix(ser):
    """Run MATRIX command and display crosstalk matrix."""
    send_cmd(ser, "MATRIX")
    lines = read_block(ser, timeout=5.0)
    js = "\n".join(lines)
    try:
        data = json.loads(js)
    except Exception:
        print(js)
        return
    mat = data["matrix"]
    print("Crosstalk matrix (ohms, null=invalid):")
    for i, row in enumerate(mat):
        row_str = " ".join(f"{('∞' if v is None else int(v)):>6}" for v in row)
        print(f"CH{i:02}: {row_str}")


def cmd_profile(ser):
    """Run PROFILE command and display channel specs."""
    send_cmd(ser, "PROFILE")
    lines = read_block(ser)
    js = "\n".join(lines)
    try:
        data = json.loads(js)
    except Exception:
        print(js)
        return
    print("Profile:")
    for ch, info in data.items():
        print(
            f"{ch:>4}: should={info['should_be_connected']} "
            f"range=[{info['expected_min']}, {info['expected_max']}] Ω"
        )


def cmd_profile_set(ser, ch, should, minv, maxv):
    """Run PROFILE_SET command to update a channel spec."""
    cmd = f"PROFILE_SET {ch} {1 if should else 0} {minv} {maxv}"
    send_cmd(ser, cmd)
    lines = read_block(ser)
    print("\n".join(lines))


def cmd_selftest(ser):
    """Run SELFTEST command."""
    send_cmd(ser, "SELFTEST")
    lines = read_block(ser)
    print("\n".join(lines))


def cmd_calibrate(ser):
    """Run CALIBRATE command."""
    send_cmd(ser, "CALIBRATE")
    lines = read_block(ser)
    print("\n".join(lines))


def main():
    p = argparse.ArgumentParser(description="Artemis Harness Tester CLI")
    p.add_argument("--port", required=True, help="Serial port (e.g. COM5, /dev/ttyUSB0)")
    p.add_argument("--baud", type=int, default=115200)

    sub = p.add_subparsers(dest="cmd", required=True)

    sub.add_parser("measure", help="Measure all channels")
    sub.add_parser("matrix", help="Measure crosstalk matrix")
    sub.add_parser("profile", help="Show current profile")

    sp = sub.add_parser("profile-set", help="Set channel profile")
    sp.add_argument("ch", type=int, help="Channel number (0-20)")
    sp.add_argument("should", type=int, choices=[0, 1], help="Should be connected (0/1)")
    sp.add_argument("min", type=float, help="Min resistance (ohms)")
    sp.add_argument("max", type=float, help="Max resistance (ohms)")

    sub.add_parser("selftest", help="Run self-test")
    sub.add_parser("calibrate", help="Run calibration")

    args = p.parse_args()

    ser = serial.Serial(args.port, args.baud, timeout=1)
    time.sleep(0.5)  # let board reset if needed
    ser.reset_input_buffer()

    if args.cmd == "measure":
        cmd_measure(ser)
    elif args.cmd == "matrix":
        cmd_matrix(ser)
    elif args.cmd == "profile":
        cmd_profile(ser)
    elif args.cmd == "profile-set":
        cmd_profile_set(ser, args.ch, bool(args.should), getattr(args, 'min'), getattr(args, 'max'))
    elif args.cmd == "selftest":
        cmd_selftest(ser)
    elif args.cmd == "calibrate":
        cmd_calibrate(ser)


if __name__ == "__main__":
    main()
