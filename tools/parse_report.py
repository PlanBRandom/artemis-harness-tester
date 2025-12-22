#!/usr/bin/env python3
"""
Artemis Harness Tester - Report Parser
Reads JSON test reports and displays results
"""

import json
import sys

def main():
    if len(sys.argv) < 2:
        print("Usage: python parse_report.py <report.json>")
        sys.exit(1)
    
    with open(sys.argv[1]) as f:
        report = json.load(f)
    
    harness_report = report.get("harness_test_report", {})
    
    print(f"Test Report v{harness_report.get('version', 'unknown')}")
    print(f"Result: {harness_report.get('result', 'UNKNOWN')}")
    print("-" * 60)
    
    for test in harness_report.get("tests", []):
        net = test.get("net", "Unknown")
        print(f"\nNet: {net}")
        
        if "continuity" in test:
            print(f"  Continuity: {test['continuity']}")
        
        if "resistance" in test:
            res = test["resistance"]
            if isinstance(res, dict):
                print(f"  Resistance: {res['status']} (ratio: {res['ratio']:.3f})")
            else:
                print(f"  Resistance: {res}")
        
        if "leakage" in test:
            leak = test["leakage"]
            if isinstance(leak, dict):
                print(f"  Leakage: {leak['status']} (ratio: {leak['ratio']:.3f})")
            else:
                print(f"  Leakage: {leak}")
        
        if "zener" in test:
            zener = test["zener"]
            if isinstance(zener, dict):
                print(f"  Zener: {zener['status']} (voltage: {zener['voltage']:.3f}V)")
            else:
                print(f"  Zener: {zener}")

if __name__ == "__main__":
    main()