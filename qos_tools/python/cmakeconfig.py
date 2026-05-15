#!/usr/bin/env python3

import argparse
import sys

import kconfiglib

DESCRIPTION = """
Generates a CMake file with defines from the configuration.
"""

def main():
    parser = argparse.ArgumentParser(description=DESCRIPTION)

    parser.add_argument(
        "--kconfig",
        metavar="KCONFIG_FILENAME",
        dest="kconfig",
        default="Kconfig",
        help="top-level Kconfig file (default: Kconfig)")

    parser.add_argument(
        "config",
        metavar="CONFIG_FILENAME",
        help="configuration file name")

    parser.add_argument(
        "cmake",
        metavar="OUTPUT_FILENAME",
        help="generated CMake file name")

    args = parser.parse_args()

    kconf = kconfiglib.Kconfig(args.kconfig)

    kconf.load_config(args.config)

    kconf.write_cmakeconf(args.cmake)

if __name__ == "__main__":
    main()
