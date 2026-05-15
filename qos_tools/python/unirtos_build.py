#!/usr/bin/env python3

import sys

from unirtos_common import (
    EXIT_OK,
    EXIT_UNSUPPORTED_COMMAND,
    BuildError,
    CommandStepError,
    eprint,
)
from unirtos_make import handle_make
from unirtos_project import handle_project
from unirtos_variant import handle_internal_sdk_derive


def print_general_usage():
    print("Usage:")
    print("  unirtos.exe --version")
    print("  unirtos.exe menuconfig")
    print("  unirtos.exe make <project> <version> [all|app] [new|r]")
    print("  unirtos.exe make --project <project> --version <version> [--type all|app] [--operation new|r] [--jobs N]")
    print("  unirtos.exe project create <project_name> [--from-demo <demo_name>]")
    print("")
    print("Commands:")
    print("  --version   Show UniRTOS tool version")
    print("  menuconfig  Open SDK configuration UI")
    print("  make        Build SDK application or full package")
    print("  project     Create an external application project under qos_applications")
    print("")
    print("Use `unirtos.exe make --help` to see make-specific help.")
    print("Use `unirtos.exe project --help` to see project-specific help.")


def main(argv):
    if not argv or argv[0] in {"-h", "--help", "help"}:
        print_general_usage()
        return EXIT_OK

    # Keep the top-level entry thin; command-specific behavior lives in dedicated modules.
    command = argv[0]
    if command == "make":
        handler = handle_make
        error_prefix = "[unirtos make]"
    elif command == "project":
        handler = handle_project
        error_prefix = "[unirtos project]"
    elif command == "internal-sdk-derive":
        handler = handle_internal_sdk_derive
        error_prefix = "[unirtos internal-sdk-derive]"
    else:
        eprint(f"unsupported command: {command}")
        print_general_usage()
        return EXIT_UNSUPPORTED_COMMAND

    try:
        return handler(argv[1:])
    except CommandStepError as exc:
        cmd_text = " ".join(str(part) for part in exc.command)
        eprint(f"{error_prefix} {exc}")
        eprint(f"{error_prefix} failed command: {cmd_text}")
        return exc.exit_code
    except BuildError as exc:
        eprint(f"{error_prefix} {exc}")
        return exc.exit_code


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
