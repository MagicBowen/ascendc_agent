import json
import sys

try:
    input_data = json.load(sys.stdin)
except json.JSONDecodeError as e:
    print(f"Error: Invalid JSON input: {e}", file=sys.stderr)
    sys.exit(1)

LOG_FILE = "/Users/wangbo/codes/agent-spike/ascendc_agent/tests/input_data_log.jsonl"

try:
    with open(LOG_FILE, "a", encoding="utf-8") as f:
        json.dump(input_data, f, ensure_ascii=False)
        f.write("\n")
except IOError as e:
    print(f"Error: Failed to write to log file: {e}", file=sys.stderr)
    sys.exit(1)