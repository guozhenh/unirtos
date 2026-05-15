#!/usr/bin/env python3
import argparse
import re
import sys

NOT_SET_RE = re.compile(r"^#\s*(CONFIG_[A-Za-z0-9_]+)\s+is not set\s*$")
SET_RE = re.compile(r"^(CONFIG_[A-Za-z0-9_]+)=(.*)$")
INT_RE = re.compile(
    r"^[-+]?(?:0[xX][0-9a-fA-F]+|[0-9]+)(?:[uUlL]+)?$"
)
TRISTATE_ORDER = {"n": 0, "m": 1, "y": 2}


def _parse_value(raw_value):
    value = raw_value.strip()
    if value in TRISTATE_ORDER:
        return ("tristate", value)

    if INT_RE.match(value):
        normalized = re.sub(r"[uUlL]+$", "", value)
        try:
            return ("int", int(normalized, 0))
        except ValueError:
            pass

    return ("raw", value)


def parse_config(path):
    symbols = {}
    with open(path, "r", encoding="utf-8", errors="ignore") as f:
        for lineno, line in enumerate(f, start=1):
            text = line.strip()
            if not text:
                continue

            m_not_set = NOT_SET_RE.match(text)
            if m_not_set:
                name = m_not_set.group(1)
                symbols[name] = {
                    "type": "tristate",
                    "value": "n",
                    "raw": "n",
                    "line": lineno,
                }
                continue

            m_set = SET_RE.match(text)
            if not m_set:
                continue

            name = m_set.group(1)
            raw_value = m_set.group(2).strip()
            value_type, value = _parse_value(raw_value)
            symbols[name] = {
                "type": value_type,
                "value": value,
                "raw": raw_value,
                "line": lineno,
            }

    return symbols


def check_capability(check_path, user_path):
    check_symbols = parse_config(check_path)
    user_symbols = parse_config(user_path)

    errors = []

    for name, user_item in user_symbols.items():
        check_item = check_symbols.get(name)
        if not check_item:
            continue

        # For tristate/bool, user value rank cannot exceed capability rank.
        if user_item["type"] == "tristate" and check_item["type"] == "tristate":
            user_rank = TRISTATE_ORDER[user_item["value"]]
            check_rank = TRISTATE_ORDER[check_item["value"]]
            if user_rank > check_rank:
                errors.append(
                    f"{name}: user '{user_item['raw']}' (line {user_item['line']}) "
                    f"> capability '{check_item['raw']}' (line {check_item['line']})"
                )
            continue

        # For numeric capability, user numeric value cannot exceed capability value.
        if user_item["type"] == "int" and check_item["type"] == "int":
            if user_item["value"] > check_item["value"]:
                errors.append(
                    f"{name}: user '{user_item['raw']}' (line {user_item['line']}) "
                    f"> capability '{check_item['raw']}' (line {check_item['line']})"
                )
            continue

    return errors


def main():
    parser = argparse.ArgumentParser(
        description="Check whether user .config stays within check.config capability."
    )
    parser.add_argument("check_config", help="Capability config file (check.config)")
    parser.add_argument("user_config", help="User config file (.config)")
    args = parser.parse_args()

    errors = check_capability(args.check_config, args.user_config)
    if errors:
        print("[check_config_cap] ERROR: Found configuration capability violations:")
        for item in errors:
            print(f"  - {item}")
        print("[check_config_cap] HINT: Reduce value or disable feature in root .config.")
        return 1

    print("[check_config_cap] Capability check passed.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
