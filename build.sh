#!/usr/bin/env sh

set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
PYTHON_BIN="${PY3:-python}"

exec "$PYTHON_BIN" "$SCRIPT_DIR/qos_tools/python/unirtos_build.py" "$@"
