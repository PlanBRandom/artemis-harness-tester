import serial
import time
import csv
from datetime import datetime

PORT = "COM5"
BAUD = 115200

def wait_for_csv_block(ser):
    lines = []
    in_block = False

    while True:
        line = ser.readline().decode(errors="ignore").strip()
        if not line:
            continue

        print(line)

        if line.startswith("CSV_MATRIX_BEGIN"):
            in_block = True
            lines = []
            continue

        if line.startswith("CSV_MATRIX_END"):
            break

        if in_block:
            lines.append(line)

    return lines

def parse_csv_matrix(lines):
    rows = []
    for line in lines:
        parts = [p.strip() for p in line.split(",")]
        name = parts[0]
        values = [int(v) for v in parts[1:]]
        rows.append((name, values))
    return rows

def save_csv(rows, filename):
    with open(filename, "w", newline="") as f:
        writer = csv.writer(f)
        header = ["Channel"] + [r[0] for r in rows]
        writer.writerow(header)
        for name, vals in rows:
            writer.writerow([name] + vals)

def main():
    print("Connecting to Artemis on", PORT)
    ser = serial.Serial(PORT, BAUD, timeout=1)
    time.sleep(2)

    print("Waiting for test run...\n")

    lines = wait_for_csv_block(ser)
    rows = parse_csv_matrix(lines)

    ts = datetime.now().strftime("%Y%m%d_%H%M%S")
    filename = f"harness_matrix_{ts}.csv"
    save_csv(rows, filename)

    print("\nSaved matrix to", filename)
    print("Done.")

if __name__ == "__main__":
    main()
