#!/usr/bin/env python3

import argparse
import sys
import os


def main(argv):
    parser = argparse.ArgumentParser(
        description='generate group linker script')
    parser.add_argument('--base', dest='base', default=None,
                        help='base directory of libraries')
    parser.add_argument('output', help='output file name')
    parser.add_argument('libs', nargs='+',
                        help='library file names')

    args = parser.parse_args(argv)

    libs = args.libs
    if args.base:
        libs = [os.path.relpath(x, args.base) for x in libs]

    with open(args.output, 'w') as fh:
        fh.write('GROUP(\n' + '\n'.join(libs) + ')\n')
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
