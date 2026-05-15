#!/usr/bin/env python3

import argparse
import json
import os
import re
import sys


ENTRY_PATTERN = re.compile(
    r'UNIRTOS_APP_EXPORT\(\s*(?P<order>\d+)\s*,\s*"(?P<entry_name>[^"]+)"\s*,\s*(?P<function>[A-Za-z_][A-Za-z0-9_]*)\s*\)'
)


def load_manifests(apps_root):
    manifests = {}
    manifest_paths = []
    for entry in sorted(os.listdir(apps_root)):
        manifest_path = os.path.join(apps_root, entry, "demo.manifest.json")
        if not os.path.isfile(manifest_path):
            continue
        with open(manifest_path, "r", encoding="utf-8") as handle:
            manifest = json.load(handle)
        manifest["_dir"] = entry
        manifest["_path"] = manifest_path
        manifests[entry] = manifest
        manifest_paths.append(manifest_path)
    return manifests, manifest_paths


def parse_enabled_entries(entries_path):
    enabled_entries = []
    with open(entries_path, "r", encoding="utf-8") as handle:
        for raw_line in handle:
            stripped = raw_line.lstrip()
            if stripped.startswith("//"):
                continue
            match = ENTRY_PATTERN.search(raw_line)
            if not match:
                continue
            enabled_entries.append(
                {
                    "order": int(match.group("order")),
                    "entry_name": match.group("entry_name"),
                    "function": match.group("function"),
                }
            )
    return enabled_entries


def manifest_supports_platform(manifest, platform):
    platforms = manifest.get("platforms", ["all"])
    return "all" in platforms or platform in platforms


def resolve_selected_manifests(manifests, enabled_entries, platform):
    entry_index = {}
    for manifest in manifests.values():
        for entry in manifest.get("entries", []):
            key = entry["entry_name"]
            if key in entry_index:
                raise RuntimeError(f"Duplicate entry_name in manifests: {key}")
            entry_index[key] = (manifest, entry)

    selected_demo_dirs = []
    selected_build_dirs = []
    selected_depend_configs = []
    selected_entry_names = []
    selected_entries = []
    seen_demo_dirs = set()
    seen_build_dirs = set()
    seen_depend_configs = set()

    for enabled_entry in sorted(enabled_entries, key=lambda item: item["order"]):
        if enabled_entry["entry_name"] not in entry_index:
            raise RuntimeError(f"Enabled init entry not found in manifests: {enabled_entry['entry_name']}")

        manifest, manifest_entry = entry_index[enabled_entry["entry_name"]]
        if manifest_entry["function"] != enabled_entry["function"]:
            raise RuntimeError(
                f"Init entry function mismatch for {enabled_entry['entry_name']}: "
                f"entries.def={enabled_entry['function']} manifest={manifest_entry['function']}"
            )

        if not manifest_supports_platform(manifest, platform):
            raise RuntimeError(
                f"Init entry {enabled_entry['entry_name']} does not support platform {platform}"
            )

        selected_entries.append(
            {
                "order": enabled_entry["order"],
                "entry_name": enabled_entry["entry_name"],
                "function": enabled_entry["function"],
            }
        )

        manifest_dir = manifest["_dir"]
        if manifest_dir not in seen_demo_dirs:
            selected_demo_dirs.append(manifest_dir)
            seen_demo_dirs.add(manifest_dir)

        for build_dir in manifest.get("build_dirs", [manifest_dir]):
            if build_dir not in seen_build_dirs:
                selected_build_dirs.append(build_dir)
                seen_build_dirs.add(build_dir)

        depend_config = manifest.get("depend_config")
        if depend_config:
            depend_config_path = os.path.join(os.path.dirname(manifest["_path"]), depend_config)
            if os.path.isfile(depend_config_path):
                normalized = os.path.abspath(depend_config_path)
                if normalized not in seen_depend_configs:
                    selected_depend_configs.append(normalized)
                    seen_depend_configs.add(normalized)

        selected_entry_names.append(enabled_entry["entry_name"])

    return {
        "demo_dirs": selected_demo_dirs,
        "build_dirs": selected_build_dirs,
        "depend_configs": selected_depend_configs,
        "entry_names": selected_entry_names,
        "entries": selected_entries,
    }


def cmake_quote(path):
    return path.replace("\\", "/")


def write_cmake(path, manifest_paths, selected):
    with open(path, "w", encoding="utf-8", newline="\n") as handle:
        handle.write("set(ALL_APP_DEMO_MANIFESTS\n")
        for manifest_path in manifest_paths:
            handle.write(f'    "{cmake_quote(os.path.abspath(manifest_path))}"\n')
        handle.write(")\n\n")

        handle.write("set(SELECTED_APP_DEMO_DIRS\n")
        for demo_dir in selected["demo_dirs"]:
            handle.write(f'    "{demo_dir}"\n')
        handle.write(")\n\n")

        handle.write("set(SELECTED_APP_BUILD_DIRS\n")
        for build_dir in selected["build_dirs"]:
            handle.write(f'    "{build_dir}"\n')
        handle.write(")\n\n")

        handle.write("set(SELECTED_APP_DEMO_ENTRY_NAMES\n")
        for entry_name in selected["entry_names"]:
            handle.write(f'    "{entry_name}"\n')
        handle.write(")\n\n")

        handle.write("set(SELECTED_APP_DEMO_DEPEND_CONFIGS\n")
        for depend_config in selected["depend_configs"]:
            handle.write(f'    "{cmake_quote(depend_config)}"\n')
        handle.write(")\n")


def write_registration_c(path, selected_entries):
    with open(path, "w", encoding="utf-8", newline="\n") as handle:
        handle.write('#include "unirtos_app_init_registry.h"\n\n')

        for entry in selected_entries:
            handle.write(f'extern void {entry["function"]}(void);\n')

        if selected_entries:
            handle.write("\n")

        for entry in selected_entries:
            handle.write(
                f'UNIRTOS_APP_EXPORT({entry["order"]}, "{entry["entry_name"]}", {entry["function"]});\n'
            )


def main():
    parser = argparse.ArgumentParser(description="Resolve selected app demos from apps_init_entries.def and demo manifests.")
    parser.add_argument("--apps-root", required=True, help="qos_applications/apps directory")
    parser.add_argument("--entries", required=True, help="apps_init_entries.def path")
    parser.add_argument(
        "--platform",
        required=True,
        help="current build arch/platform, matching qos_kernel directory names such as eigen_718 or uis8852",
    )
    parser.add_argument("--output-cmake", required=True, help="generated CMake include path")
    parser.add_argument("--output-registration-c", required=True, help="generated C source for runtime app init registration")
    args = parser.parse_args()

    try:
        manifests, manifest_paths = load_manifests(os.path.abspath(args.apps_root))
        enabled_entries = parse_enabled_entries(os.path.abspath(args.entries))
        selected = resolve_selected_manifests(manifests, enabled_entries, args.platform)
        write_cmake(os.path.abspath(args.output_cmake), manifest_paths, selected)
        write_registration_c(os.path.abspath(args.output_registration_c), selected["entries"])
    except Exception as exc:
        print(f"[resolve_demo_selection] {exc}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
