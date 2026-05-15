#!/usr/bin/env python3

import argparse
import os
import sys

import kconfiglib


def main():
    parser = argparse.ArgumentParser(
        description="Generate a default .config from Kconfig defaults."
    )
    parser.add_argument(
        "--kconfig",
        default="Kconfig",
        help="Top-level Kconfig file (default: Kconfig)",
    )
    parser.add_argument(
        "--output",
        required=True,
        help="Output .config path",
    )
    args = parser.parse_args()

    try:
        kconf = kconfiglib.Kconfig(args.kconfig)

        output_path = os.path.abspath(args.output)
        output_dir = os.path.dirname(output_path)
        if output_dir:
            os.makedirs(output_dir, exist_ok=True)

        kconf.write_config(output_path, save_old=False, verbose=False)
        print(
            f"[gen_default_config] Generated default config from "
            f"'{args.kconfig}' to '{output_path}'"
        )
        return 0
    except Exception as exc:
        print(f"[gen_default_config] ERROR: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
