#!/usr/bin/env python3

from dataclasses import dataclass
from pathlib import Path
import re
import shutil
from typing import Optional

from unirtos_common import (
    EXIT_ARGUMENT_ERROR,
    EXIT_PROJECT_COPY_ERROR,
    EXIT_PROJECT_CREATE_ERROR,
    EXIT_PROJECT_TEMPLATE_ERROR,
    BuildError,
    project_info,
    sdk_root,
)


PROJECT_NAME_RE = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")
TARGET_PATTERN = re.compile(r"set\s*\(\s*target\s+[A-Za-z0-9_]+\s*\)", re.IGNORECASE)
INIT_EXPORT_PATTERN = re.compile(r"\bUNIRTOS_APP_EXPORT\s*\(")


@dataclass(frozen=True)
class ProjectCreateConfig:
    project_name: str
    from_demo: Optional[str] = None

    @property
    def target_dir(self) -> Path:
        return sdk_root() / "qos_applications" / self.project_name


def print_project_usage():
    print("Usage:")
    print("  unirtos.exe project <command> [options]")
    print("")
    print("Commands:")
    print("  create    Create an external application project under qos_applications")
    print("")
    print("Examples:")
    print("  unirtos.exe project create dtu_app")
    print("  unirtos.exe project create dtu_app --from-demo easy_ftp_demos")
    print("")
    print("Use `unirtos.exe project create --help` to see create-specific help.")


def print_project_create_usage():
    print("Usage:")
    print("  unirtos.exe project create <project_name> [--from-demo <demo_name>]")
    print("")
    print("Examples:")
    print("  unirtos.exe project create dtu_app")
    print("  unirtos.exe project create dtu_app --from-demo easy_ftp_demos")


def parse_project_args(args):
    if not args or args[0] in {"-h", "--help", "help"}:
        print_project_usage()
        return None

    subcommand = args[0]
    if subcommand != "create":
        print_project_usage()
        raise BuildError(f"unsupported project subcommand: {subcommand}", EXIT_ARGUMENT_ERROR)

    config = parse_project_create_args(args[1:])
    return ("create", config)


def parse_project_create_args(args):
    if not args or args[0] in {"-h", "--help", "help"}:
        print_project_create_usage()
        return None

    project_name = None
    from_demo = None
    positional = []

    i = 0
    while i < len(args):
        arg = args[i]
        if arg in {"-h", "--help", "help"}:
            print_project_create_usage()
            return None
        if arg == "--from-demo":
            i += 1
            if i >= len(args):
                raise BuildError("--from-demo requires a value", EXIT_ARGUMENT_ERROR)
            from_demo = args[i]
        elif arg.startswith("--"):
            raise BuildError(f"unknown option: {arg}", EXIT_ARGUMENT_ERROR)
        else:
            positional.append(arg)
        i += 1

    if len(positional) != 1:
        raise BuildError("project create requires exactly one <project_name>", EXIT_ARGUMENT_ERROR)

    project_name = positional[0]
    validate_project_name(project_name)

    if from_demo:
        validate_project_name(from_demo)

    return ProjectCreateConfig(project_name=project_name, from_demo=from_demo)


def validate_project_name(name: str):
    if not PROJECT_NAME_RE.match(name):
        raise BuildError(
            f"invalid project name: {name} (use letters, digits, and underscore; cannot start with a digit)",
            EXIT_PROJECT_CREATE_ERROR,
        )


def ensure_target_dir_available(target_dir: Path):
    if target_dir.exists():
        raise BuildError(f"target project already exists: {target_dir}", EXIT_PROJECT_CREATE_ERROR)


def source_demo_dir(demo_name: str) -> Path:
    return sdk_root() / "qos_applications" / "apps" / demo_name


def ensure_demo_exists(demo_name: str) -> Path:
    demo_dir = source_demo_dir(demo_name)
    if not demo_dir.exists() or not demo_dir.is_dir():
        raise BuildError(f"demo not found: {demo_name}", EXIT_PROJECT_CREATE_ERROR)
    cmakelists_path = demo_dir / "CMakeLists.txt"
    if not cmakelists_path.exists():
        raise BuildError(f"demo does not contain CMakeLists.txt: {demo_name}", EXIT_PROJECT_CREATE_ERROR)
    return demo_dir


def write_text_file(path: Path, content: str):
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", encoding="utf-8", newline="\n") as fp:
        fp.write(content)


def render_empty_project_cmakelists(project_name: str) -> str:
    return f"""message(STATUS "cmake config ${{CMAKE_CURRENT_SOURCE_DIR}}")

set(target {project_name})
add_apps_libraries($<TARGET_FILE:${{target}}>)
add_library(${{target}} STATIC)
set_target_properties(${{target}} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${{out_unir_lib_dir}})

target_include_directories(${{target}} PRIVATE ${{SOURCE_TOP_DIR}})
target_include_directories(${{target}} PUBLIC ${{CMAKE_CURRENT_SOURCE_DIR}}/inc)
target_include_directories(${{target}} PUBLIC ${{CMAKE_CURRENT_SOURCE_DIR}}/src)

target_sources(${{target}} PRIVATE
    src/{project_name}.c
    src/{project_name}_init.c
)

target_link_libraries_if(CONFIG_QOSA_LINK_SYSTEM_LIBRARIES_FUNC THEN ${{target}} system_os)
"""


def render_empty_project_header(project_name: str) -> str:
    guard = f"__{project_name.upper()}_H__"
    return f"""#ifndef {guard}
#define {guard}

void {project_name}_start(void);

#endif /* {guard} */
"""


def render_empty_project_source(project_name: str) -> str:
    return f"""#include "{project_name}.h"
#include "qosa_log.h"

#define QOS_LOG_TAG LOG_TAG_DEMO

void {project_name}_start(void)
{{
    QLOGI("{project_name} started");
}}
"""


def render_empty_project_init(project_name: str) -> str:
    return f"""#include "{project_name}.h"
#include "unirtos_app_init_registry.h"

static void {project_name}_app_init(void)
{{
    {project_name}_start();
}}

UNIRTOS_APP_EXPORT(700, "{project_name}", {project_name}_app_init);
"""


def render_depend_config(project_name: str) -> str:
    return f"""# Capability dependencies for {project_name}
# Add CONFIG_QCM_* / CONFIG_QOSA_* items here as needed.
# Do not place CONFIG_QAPP_* demo macros in this file.
"""


def render_readme(project_name: str) -> str:
    return f"""# {project_name}

This project lives directly under `qos_applications/` and is auto-discovered by the SDK build.

## Files

- `CMakeLists.txt`: project build entry
- `depend.config`: capability dependencies merged into the build-time config
- `src/{project_name}_init.c`: app init registration via `UNIRTOS_APP_EXPORT`

## Next Steps

1. Add capability dependencies to `depend.config`.
2. Implement your application logic in `src/{project_name}.c`.
3. Build with the normal `unirtos.exe make ...` flow.
"""


def create_empty_project(config: ProjectCreateConfig):
    target_dir = config.target_dir
    ensure_target_dir_available(target_dir)
    target_dir.mkdir(parents=True, exist_ok=False)

    write_text_file(target_dir / "CMakeLists.txt", render_empty_project_cmakelists(config.project_name))
    write_text_file(target_dir / "depend.config", render_depend_config(config.project_name))
    write_text_file(target_dir / "inc" / f"{config.project_name}.h", render_empty_project_header(config.project_name))
    write_text_file(target_dir / "src" / f"{config.project_name}.c", render_empty_project_source(config.project_name))
    write_text_file(target_dir / "src" / f"{config.project_name}_init.c", render_empty_project_init(config.project_name))
    write_text_file(target_dir / "README.md", render_readme(config.project_name))

    project_info(f"created project: {target_dir}")
    project_info("init registration has been added automatically")
    project_info("edit depend.config to add capability dependencies")


def rewrite_cmakelists_target(cmakelists_path: Path, project_name: str):
    content = cmakelists_path.read_text(encoding="utf-8")
    updated_content, replace_count = TARGET_PATTERN.subn(f"set(target {project_name})", content, count=1)

    if replace_count == 0:
        raise BuildError(
            f"failed to rewrite target name in {cmakelists_path}, expected a 'set(target ...)' entry",
            EXIT_PROJECT_TEMPLATE_ERROR,
        )

    with cmakelists_path.open("w", encoding="utf-8", newline="\n") as fp:
        fp.write(updated_content)


def ensure_depend_config_exists(target_dir: Path, project_name: str):
    depend_config_path = target_dir / "depend.config"
    if depend_config_path.exists():
        return False

    write_text_file(depend_config_path, render_depend_config(project_name))
    return True


def project_contains_init_registration(target_dir: Path) -> bool:
    for source_file in target_dir.rglob("*.[ch]"):
        try:
            content = source_file.read_text(encoding="utf-8", errors="ignore")
        except OSError:
            continue
        if INIT_EXPORT_PATTERN.search(content):
            return True
    return False


def copy_demo_project(config: ProjectCreateConfig):
    demo_dir = ensure_demo_exists(config.from_demo)
    target_dir = config.target_dir
    ensure_target_dir_available(target_dir)

    try:
        shutil.copytree(demo_dir, target_dir)
    except OSError as exc:
        raise BuildError(f"failed to copy demo project: {exc}", EXIT_PROJECT_COPY_ERROR) from exc

    rewrite_cmakelists_target(target_dir / "CMakeLists.txt", config.project_name)
    created_depend_config = ensure_depend_config_exists(target_dir, config.project_name)
    has_init_registration = project_contains_init_registration(target_dir)

    project_info(f"created project from demo: {target_dir}")
    project_info(f"source demo: {config.from_demo}")
    if created_depend_config:
        project_info("depend.config was missing in the source demo, created an empty file")
    else:
        project_info("copied depend.config from the source demo")

    if has_init_registration:
        project_info("detected existing init registration, but you should still verify it matches the new project entry")
    else:
        project_info("no UNIRTOS_APP_EXPORT registration was detected, add init registration manually if needed")


def handle_project(args):
    parsed = parse_project_args(args)
    if parsed is None:
        return 0

    subcommand, config = parsed
    if config is None:
        return 0

    if subcommand != "create":
        raise BuildError(f"unsupported project subcommand: {subcommand}", EXIT_ARGUMENT_ERROR)

    if config.from_demo:
        copy_demo_project(config)
    else:
        create_empty_project(config)

    return 0
