#!/usr/bin/env python3

import argparse
import json
import os
import re
from collections import defaultdict


ENTRY_PATTERN = re.compile(
    r'APP_INIT_(?:DIRECT|WRAPPER)\(\s*(?P<order>\d+)\s*,\s*"(?P<entry_name>[^"]+)"\s*,\s*(?P<function>[A-Za-z_][A-Za-z0-9_]*)\s*\)'
)

WRAPPER_ENTRY_MAP = {
    "unir_socket_demo_bootstrap": [
        {"order": 280, "entry_name": "socket_qcm_block_demo", "function": "unir_qcm_socket_block_demo_init"},
        {"order": 281, "entry_name": "socket_qcm_noblock_demo", "function": "unir_qcm_socket_noblock_demo_init"},
        {"order": 282, "entry_name": "socket_block_demo", "function": "unir_socket_block_demo_init"},
        {"order": 283, "entry_name": "socket_noblock_demo", "function": "unir_socket_noblock_demo_init"},
    ],
    "unir_vtls_demo_bootstrap": [
        {"order": 310, "entry_name": "vtls_block_demo", "function": "unir_vtls_block_demo_init"},
        {"order": 311, "entry_name": "vtls_noblock_demo", "function": "unir_vtls_noblock_demo_init"},
        {"order": 312, "entry_name": "vtls_noblock_demo2", "function": "unir_vtls_noblock_demo2_init"},
        {"order": 313, "entry_name": "vtls_adapter_demo", "function": "unir_vtls_adapter_demo_init"},
    ],
    "unir_qurl_http_demo_bootstrap": [
        {"order": 360, "entry_name": "qurl_http_get_demo", "function": "unir_qurl_http_get_demo_init"},
        {"order": 361, "entry_name": "qurl_http_auth_demo", "function": "unir_qurl_http_auth_demo_init"},
        {"order": 362, "entry_name": "qurl_http_redirect_demo", "function": "unir_qurl_http_redirect_demo_init"},
        {"order": 363, "entry_name": "qurl_https_demo", "function": "unir_qurl_https_demo_init"},
        {"order": 364, "entry_name": "qurl_http_post_demo", "function": "unir_qurl_http_post_demo_init"},
    ],
    "unir_qurl_ftp_demo_bootstrap": [
        {"order": 380, "entry_name": "qurl_ftp_demo", "function": "unir_qurl_ftp_demo_init"},
        {"order": 381, "entry_name": "qurl_ftps_demo", "function": "unir_qurl_ftps_demo_init"},
    ],
    "unir_mqtt_demo_bootstrap": [
        {"order": 410, "entry_name": "easy_mqtt_demo", "function": "unir_easy_mqtt_demo_init"},
        {"order": 411, "entry_name": "mqtt5_demo", "function": "unir_mqtt5_demo_init"},
    ],
    "unir_mqtt_ali_demo_bootstrap": [
        {"order": 420, "entry_name": "easy_aliot_basic_demo", "function": "unir_easy_aliot_basic_demo_init"},
        {"order": 421, "entry_name": "easy_aliot_dynreghttp_demo", "function": "unir_easy_aliot_dynreghttp_demo_init"},
        {"order": 422, "entry_name": "easy_aliot_dynregmqtt_demo", "function": "unir_easy_aliot_dynregmqtt_demo_init"},
        {"order": 423, "entry_name": "easy_aliot_dynreg_demo", "function": "unir_easy_aliot_dynreg_demo_init"},
    ],
    "unir_cmsis_os2_test_bootstrap": [
        {"order": 460, "entry_name": "cmsis_os2_event_test", "function": "osEventFlagsTest_init"},
        {"order": 461, "entry_name": "cmsis_os2_message_queue_test", "function": "osMessageQueueTest_init"},
        {"order": 462, "entry_name": "cmsis_os2_mutex_test", "function": "osMutexTest_init"},
        {"order": 463, "entry_name": "cmsis_os2_semaphore_test", "function": "osSemaphoreTest_init"},
        {"order": 464, "entry_name": "cmsis_os2_thread_create_test", "function": "osThreadCreateTest_init"},
        {"order": 465, "entry_name": "cmsis_os2_timer_test", "function": "osTimerTest_init"},
    ],
    "unir_easy_fota_demo_bootstrap": [
        {"order": 520, "entry_name": "easy_fota_demo", "function": "unir_easy_fota_demo_init"},
        {"order": 521, "entry_name": "easy_fota_result_demo", "function": "unir_easy_fota_result_demo_init"},
    ],
}

IGNORED_WRAPPERS = {
    "unir_unirtos_std_bootstrap",
    "unir_cmsis_os2_bootstrap",
}

DIR_EXTRA_BUILD_DIRS = {
    "cmsis_demos": ["cmsis_demos", "cmsis_os"],
}


def parse_source_entries(path):
    entries = []
    with open(path, "r", encoding="utf-8") as handle:
        for line in handle:
            match = ENTRY_PATTERN.search(line)
            if not match:
                continue
            entries.append(
                {
                    "order": int(match.group("order")),
                    "entry_name": match.group("entry_name"),
                    "function": match.group("function"),
                }
            )
    return entries


def find_demo_dir_for_function(apps_root, function_name):
    needle = f"{function_name}("
    for dirpath, _, filenames in os.walk(apps_root):
        if os.path.basename(dirpath) == "config":
            continue
        for filename in filenames:
            if not filename.endswith(".c"):
                continue
            file_path = os.path.join(dirpath, filename)
            with open(file_path, "r", encoding="utf-8", errors="ignore") as handle:
                if needle in handle.read():
                    rel_dir = os.path.relpath(os.path.dirname(file_path), apps_root)
                    return rel_dir.split(os.sep)[0]
    return None


def build_manifest_data(apps_root, source_entries):
    grouped_entries = defaultdict(list)

    for entry in source_entries:
        wrapper_entries = WRAPPER_ENTRY_MAP.get(entry["function"])
        if wrapper_entries is not None:
            for wrapper_entry in wrapper_entries:
                demo_dir = find_demo_dir_for_function(apps_root, wrapper_entry["function"])
                if demo_dir is None:
                    raise RuntimeError(f"Unable to locate demo directory for {wrapper_entry['function']}")
                grouped_entries[demo_dir].append(wrapper_entry)
            continue

        if entry["function"] in IGNORED_WRAPPERS:
            continue

        demo_dir = find_demo_dir_for_function(apps_root, entry["function"])
        if demo_dir is None:
            raise RuntimeError(f"Unable to locate demo directory for {entry['function']}")
        grouped_entries[demo_dir].append(entry)

    manifests = {}
    for demo_dir, entries in grouped_entries.items():
        manifests[demo_dir] = {
            "name": demo_dir,
            "type": "demo",
            "platforms": ["all"],
            "depend_config": "depend.config",
            "entries": sorted(entries, key=lambda item: item["order"]),
        }
        build_dirs = DIR_EXTRA_BUILD_DIRS.get(demo_dir)
        if build_dirs:
            manifests[demo_dir]["build_dirs"] = build_dirs
    return manifests


def write_manifests(apps_root, manifests):
    for demo_dir, manifest in manifests.items():
        manifest_path = os.path.join(apps_root, demo_dir, "demo.manifest.json")
        with open(manifest_path, "w", encoding="utf-8", newline="\n") as handle:
            json.dump(manifest, handle, indent=2)
            handle.write("\n")


def write_entries_template(path, manifests):
    ordered_entries = []
    for manifest in manifests.values():
        for entry in manifest["entries"]:
            ordered_entries.append(entry)
    ordered_entries.sort(key=lambda item: item["order"])

    with open(path, "w", encoding="utf-8", newline="\n") as handle:
        handle.write("/* Uncomment the demo init entries that should participate in build and runtime registration. */\n")
        for entry in ordered_entries:
            handle.write(
                f'// UNIRTOS_APP_EXPORT({entry["order"]}, "{entry["entry_name"]}", {entry["function"]})\n'
            )


def main():
    parser = argparse.ArgumentParser(description="Sync demo manifests and commented init entry template.")
    parser.add_argument("--apps-root", required=True, help="qos_applications/apps directory")
    parser.add_argument("--source-entries", required=True, help="Current apps_init_entries.def used as bootstrap source")
    parser.add_argument("--write-entries-template", required=True, help="Destination entries.def template path")
    args = parser.parse_args()

    apps_root = os.path.abspath(args.apps_root)
    source_entries = parse_source_entries(args.source_entries)
    manifests = build_manifest_data(apps_root, source_entries)
    write_manifests(apps_root, manifests)
    write_entries_template(args.write_entries_template, manifests)

    print(f"generated manifests for {len(manifests)} demos")


if __name__ == "__main__":
    main()
