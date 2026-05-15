## Dynamically update the binfile/binheader paths in qosa_secureboot_cfg_ec718pm.json
import json
import sys
import os

def fix_path(path, unirtos_build_out, binary_bin_dir):
    path = path.strip().replace('\\', '/').lstrip('/')
    if path.startswith("gccout/"):
        subpath = path[len("gccout/"):]
        return f"{unirtos_build_out}/gccout/{subpath}"
    elif path.startswith("bin/"):
        subpath = path[len("bin/"):]
        return f"{binary_bin_dir}/{subpath}"
    else:
        return path

if __name__ == "__main__":
    input_json = sys.argv[1]
    output_json = sys.argv[2]
    unirtos_build_out = sys.argv[3]
    binary_bin_dir = sys.argv[4]

    try:
        with open(input_json, 'r') as f:
            data = json.load(f)
    except Exception as e:
        print("ERROR: Failed to read input_json:", e)
        exit(1)

    for item in data.get("AdditionalBin", []):
        if "binfile" in item:
            print("  binfile before:", item["binfile"])
            item["binfile"] = fix_path(item["binfile"], unirtos_build_out, binary_bin_dir)
            print("  binfile after:", item["binfile"])
        if "binheader" in item:
            print("  binheader before:", item["binheader"])
            item["binheader"] = fix_path(item["binheader"], unirtos_build_out, binary_bin_dir)
            print("  binheader after:", item["binheader"])

    with open(output_json, 'w', encoding='utf-8') as f:
        json.dump(data, f, indent=4, ensure_ascii=False)