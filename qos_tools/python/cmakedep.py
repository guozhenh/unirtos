#!/usr/bin/env python3

import argparse
import sys

import kconfiglib

DESCRIPTION = """
Generates a CMake file with cmake configuration dependency.
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
        "cmake",
        metavar="OUTPUT_FILENAME",
        help="generated CMake file name")

    args = parser.parse_args()
    kconf = kconfiglib.Kconfig(args.kconfig)
    with open(args.cmake, 'w') as f:
        f.write('set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS\n{})\n'
                .format('\n'.join(kconf._filelist).replace('\\', '/')))


if __name__ == "__main__":
    main()
