#!/usr/bin/env python3

from dataclasses import dataclass
import json
import os
from pathlib import Path
import shutil
from typing import Optional, Tuple

from unirtos_common import (
    EXIT_ARGUMENT_ERROR,
    EXIT_DERIVE_COPY_ERROR,
    EXIT_DERIVE_MANIFEST_ERROR,
    EXIT_DERIVE_OUTPUT_ERROR,
    EXIT_DERIVE_VALIDATION_ERROR,
    BuildError,
    delete_path,
    derive_info,
    sdk_root,
    variants_dir,
)


@dataclass(frozen=True)
class DeriveCommandArgs:
    variant: str
    out_dir: Path
    force: bool = False
    manifest_path: Optional[Path] = None


@dataclass(frozen=True)
class DeriveConfig:
    variant_name: str
    source_sdk_root: Path
    output_folder_suffix: str
    remove_files: Tuple[str, ...]
    remove_dirs: Tuple[str, ...]
    remove_globs: Tuple[str, ...]


def print_internal_derive_usage():
    print("Usage:")
    print("  unirtos.exe internal-sdk-derive --variant <name> --out <dir> [--force]")
    print("  unirtos.exe internal-sdk-derive --manifest <path> --out <dir> [--force]")
    print("")
    print("Examples:")
    print("  unirtos.exe internal-sdk-derive --variant ocpu-lite --out D:\\sdk_out")
    print("  unirtos.exe internal-sdk-derive --manifest .\\variants\\ocpu-lite.json --out D:\\sdk_out --force")


def require_manifest_object(data, field_name: str):
    if not isinstance(data, dict):
        raise BuildError(f"manifest {field_name} must be an object", EXIT_DERIVE_MANIFEST_ERROR)
    return data


def require_manifest_string(value, field_name: str, *, allow_empty: bool = False) -> str:
    if not isinstance(value, str):
        raise BuildError(f"manifest {field_name} must be a string", EXIT_DERIVE_MANIFEST_ERROR)
    if not allow_empty and not value:
        raise BuildError(f"manifest {field_name} must not be empty", EXIT_DERIVE_MANIFEST_ERROR)
    return value


def require_manifest_string_list(value, field_name: str) -> Tuple[str, ...]:
    if value is None:
        return ()
    if not isinstance(value, list):
        raise BuildError(f"manifest {field_name} must be a list of strings", EXIT_DERIVE_MANIFEST_ERROR)
    items = []
    for index, item in enumerate(value):
        if not isinstance(item, str) or not item:
            raise BuildError(
                f"manifest {field_name}[{index}] must be a non-empty string",
                EXIT_DERIVE_MANIFEST_ERROR,
            )
        items.append(item)
    return tuple(items)



def resolve_output_folder_suffix(output: dict, variant_name: str) -> str:
    folder_suffix = output.get("folder_suffix")

    if "folder_name" in output:
        raise BuildError(
            "manifest output.folder_name is not supported, use output.folder_suffix instead",
            EXIT_DERIVE_MANIFEST_ERROR,
        )

    if folder_suffix is None:
        folder_suffix = f"-{variant_name}"
    else:
        folder_suffix = require_manifest_string(folder_suffix, "output.folder_suffix", allow_empty=True)

    return folder_suffix


def parse_internal_derive_args(args):
    if not args or args[0] in {"-h", "--help", "help"}:
        print_internal_derive_usage()
        return None

    variant = None
    out_dir = None
    manifest_path = None
    force = False

    i = 0
    while i < len(args):
        arg = args[i]
        if arg in {"-h", "--help", "help"}:
            print_internal_derive_usage()
            return None
        if arg == "--variant":
            i += 1
            if i >= len(args):
                raise BuildError("--variant requires a value", EXIT_ARGUMENT_ERROR)
            variant = args[i]
        elif arg == "--out":
            i += 1
            if i >= len(args):
                raise BuildError("--out requires a value", EXIT_ARGUMENT_ERROR)
            out_dir = Path(args[i])
        elif arg == "--manifest":
            i += 1
            if i >= len(args):
                raise BuildError("--manifest requires a value", EXIT_ARGUMENT_ERROR)
            manifest_path = Path(args[i])
        elif arg == "--force":
            force = True
        else:
            raise BuildError(f"unknown option: {arg}", EXIT_ARGUMENT_ERROR)
        i += 1

    if out_dir is None:
        raise BuildError("--out is required", EXIT_ARGUMENT_ERROR)
    if variant is None and manifest_path is None:
        raise BuildError("--variant or --manifest is required", EXIT_ARGUMENT_ERROR)
    if variant is not None and manifest_path is not None:
        raise BuildError("--variant and --manifest cannot be used together", EXIT_ARGUMENT_ERROR)

    if variant is None:
        variant = manifest_path.stem

    return DeriveCommandArgs(
        variant=variant,
        out_dir=out_dir,
        force=force,
        manifest_path=manifest_path,
    )


def resolve_manifest_path(root: Path, args: DeriveCommandArgs) -> Path:
    if args.manifest_path is not None:
        manifest_path = args.manifest_path
        if not manifest_path.is_absolute():
            manifest_path = (root / manifest_path).resolve()
        if not manifest_path.exists():
            raise BuildError(f"manifest not found: {manifest_path}", EXIT_DERIVE_MANIFEST_ERROR)
        return manifest_path

    for ext in (".json", ".yaml", ".yml"):
        candidate = variants_dir(root) / f"{args.variant}{ext}"
        if candidate.exists():
            return candidate

    raise BuildError(
        f"manifest not found for variant: {args.variant} (searched .json/.yaml/.yml under {variants_dir(root)})",
        EXIT_DERIVE_MANIFEST_ERROR,
    )


def load_manifest_file(manifest_path: Path):
    suffix = manifest_path.suffix.lower()
    if suffix == ".json":
        with manifest_path.open("r", encoding="utf-8") as fp:
            return json.load(fp)

    if suffix in {".yaml", ".yml"}:
        try:
            import yaml  # type: ignore
        except ImportError as exc:
            raise BuildError(
                f"yaml manifest requires PyYAML: {manifest_path}",
                EXIT_DERIVE_MANIFEST_ERROR,
            ) from exc
        with manifest_path.open("r", encoding="utf-8") as fp:
            return yaml.safe_load(fp)

    raise BuildError(f"unsupported manifest format: {manifest_path}", EXIT_DERIVE_MANIFEST_ERROR)


def manifest_to_derive_config(root: Path, data) -> DeriveConfig:
    # Manifest parsing stays strict on types so bad internal configs fail early.
    if not isinstance(data, dict):
        raise BuildError("manifest root must be an object", EXIT_DERIVE_MANIFEST_ERROR)
    if data.get("schema_version") != 1:
        raise BuildError("manifest schema_version must be 1", EXIT_DERIVE_MANIFEST_ERROR)

    variant = require_manifest_object(data.get("variant") or {}, "variant")
    source = require_manifest_object(data.get("source") or {}, "source")
    output = require_manifest_object(data.get("output") or {}, "output")
    package = require_manifest_object(data.get("package") or {}, "package")
    variant_name = variant.get("name")
    if not variant_name:
        raise BuildError("manifest variant.name is required", EXIT_DERIVE_MANIFEST_ERROR)
    variant_name = require_manifest_string(variant_name, "variant.name")

    source_root_value = source.get("sdk_root", ".")
    source_root_value = require_manifest_string(source_root_value, "source.sdk_root")
    source_sdk_root = Path(source_root_value)
    if not source_sdk_root.is_absolute():
        source_sdk_root = (root / source_sdk_root).resolve()
    if not source_sdk_root.exists():
        raise BuildError(f"source sdk root not found: {source_sdk_root}", EXIT_DERIVE_MANIFEST_ERROR)

    output_folder_suffix = resolve_output_folder_suffix(output, variant_name)
    remove_files = require_manifest_string_list(package.get("remove_files"), "package.remove_files")
    remove_dirs = require_manifest_string_list(package.get("remove_dirs"), "package.remove_dirs")
    remove_globs = require_manifest_string_list(package.get("remove_globs"), "package.remove_globs")

    return DeriveConfig(
        variant_name=variant_name,
        source_sdk_root=source_sdk_root,
        output_folder_suffix=output_folder_suffix,
        remove_files=remove_files,
        remove_dirs=remove_dirs,
        remove_globs=remove_globs,
    )


def validate_derive_output_paths(source_root: Path, target_root: Path):
    source_root = source_root.resolve()
    target_root = target_root.resolve()
    if source_root == target_root:
        raise BuildError("output path must not equal source sdk root", EXIT_DERIVE_OUTPUT_ERROR)
    if str(target_root).startswith(str(source_root) + os.sep):
        raise BuildError("output path must not be inside source sdk root", EXIT_DERIVE_OUTPUT_ERROR)


def resolve_target_root(config: DeriveConfig, out_dir: Path, root: Path) -> Path:
    # Derived SDK naming is intentionally simple during early rollout:
    # source SDK directory name + manifest-provided suffix.
    folder_name = config.source_sdk_root.name + config.output_folder_suffix
    target_root = out_dir / folder_name
    if not target_root.is_absolute():
        target_root = (root / target_root).resolve()
    return target_root


def apply_remove_rules(target_root: Path, config: DeriveConfig):
    for rel_path in config.remove_files:
        delete_path(target_root / rel_path)
    for rel_path in config.remove_dirs:
        delete_path(target_root / rel_path)
    for pattern in config.remove_globs:
        for path in target_root.glob(pattern):
            delete_path(path)


def validate_derived_sdk(target_root: Path, config: DeriveConfig):
    required_paths = [
        target_root / "build.sh",
        target_root / "qos_tools" / "python" / "unirtos_build.py",
        target_root / "qos_tools" / "python" / "menuconfig.py",
        target_root / "CMakeLists.txt",
        target_root / "Kconfig",
    ]
    missing = [path for path in required_paths if not path.exists()]
    if missing:
        missing_text = ", ".join(str(path) for path in missing)
        raise BuildError(f"derived sdk validation failed, missing: {missing_text}", EXIT_DERIVE_VALIDATION_ERROR)


def print_derive_summary(config: DeriveConfig, manifest_path: Path, target_root: Path):
    derive_info("configuration")
    print(f"  variant   : {config.variant_name}")
    print(f"  manifest  : {manifest_path}")
    print(f"  source    : {config.source_sdk_root}")
    print(f"  target    : {target_root}")


def handle_internal_sdk_derive(args):
    parsed_args = parse_internal_derive_args(args)
    if parsed_args is None:
        return 0

    root = sdk_root()
    manifest_path = resolve_manifest_path(root, parsed_args)
    manifest_data = load_manifest_file(manifest_path)
    config = manifest_to_derive_config(root, manifest_data)

    # The derive flow is: load manifest -> copy source tree -> apply pruning -> validate result.
    target_root = resolve_target_root(config, parsed_args.out_dir, root)
    validate_derive_output_paths(config.source_sdk_root, target_root)
    print_derive_summary(config, manifest_path, target_root)

    if target_root.exists():
        if not parsed_args.force:
            raise BuildError(
                f"output already exists: {target_root} (use --force to overwrite)",
                EXIT_DERIVE_OUTPUT_ERROR,
            )
        derive_info(f"removing existing output: {target_root}")
        try:
            shutil.rmtree(target_root)
        except OSError as exc:
            raise BuildError(
                f"failed to remove existing output: {target_root}: {exc}",
                EXIT_DERIVE_OUTPUT_ERROR,
            ) from exc

    derive_info("copying sdk source tree")
    try:
        shutil.copytree(
            config.source_sdk_root,
            target_root,
            ignore=shutil.ignore_patterns(".git"),
        )
    except OSError as exc:
        raise BuildError(f"failed to copy sdk tree: {exc}", EXIT_DERIVE_COPY_ERROR) from exc

    derive_info("applying package remove rules")
    apply_remove_rules(target_root, config)

    derive_info("validating derived sdk")
    validate_derived_sdk(target_root, config)

    derive_info("derive succeeded")
    print(f"[unirtos internal-sdk-derive] variant : {config.variant_name}")
    print(f"[unirtos internal-sdk-derive] output  : {target_root}")
    return 0
