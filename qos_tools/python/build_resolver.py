# -*- coding: utf-8 -*-

# Copyright (c) Quectel Wireless Solution, Co., Ltd.All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
import sys
import json
import shutil
import filecmp
import configparser

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
SDK_ROOT_DIR = os.path.abspath(os.path.join(SCRIPT_DIR, os.pardir, os.pardir))
SCRIPT_NAME = os.path.basename(__file__)

DEBUG_PRINT = True
DEBUG_ERROR = True
DEBUG_WARNING = True
DEBUG_INFO = False


def log_error(message):
    if DEBUG_PRINT and DEBUG_ERROR:
        print(f"[{SCRIPT_NAME}] {message}")


def log_warning(message):
    if DEBUG_PRINT and DEBUG_WARNING:
        print(f"[{SCRIPT_NAME}] {message}")


def log_info(message):
    if DEBUG_PRINT and DEBUG_INFO:
        print(f"[{SCRIPT_NAME}] {message}")


if os.name == 'nt':
    host_spec = ' HOST=win32'
else:
    host_spec = ' HOST=linux'

argv = sys.argv
argc = len(argv)
error_occurred = False

build_log_dir = os.path.join(SDK_ROOT_DIR, 'output', 'log')
build_log_file = 'build.log'
platform_json = os.path.join(SDK_ROOT_DIR, 'qos_components', 'system', 'platform', 'platform.json')


# error process
def error_proc(msg, prompt='CommandError', errtype='EARGV'):
    global error_occurred
    if not error_occurred:
        log_error("! %s: [%s]: `%s'" % (prompt, errtype, msg))
        error_occurred = True


# help cmd process
def help_proc():
    log_warning('Usage: unirtos <action>  [opt]\n')
    log_warning('These are common commands used in various situations:')
    log_warning('   prepare           - Do the compilation work')
    log_warning('                       used: unirtos cmake plat operation project version subver qinf certver svn modemapp [oem]')
    log_warning('   help              - Show this help page')


# clean cmd process
def clean_proc():
    cmd_str = 'make clean PY=' + argv[1]
    os.system(cmd_str)


# clean cmd process
def private_clean_proc():
    cmd_str = 'make private_clean PY=' + argv[1]
    os.system(cmd_str)


# get plat by board
def get_plat_by_board(board):
    # Check if platform.json exists.
    if not os.path.exists(platform_json):
        error_proc(platform_json, 'NotFoundError', 'ECONF')
        return ''

    with open(platform_json, 'r', encoding='utf-8') as f:
        platforms = json.load(f)
        for k, v in platforms.items():
            if board in v:
                return k
        return ''


def write_buildopt_ini():
    # Extract parameters in order, where oem is optional (may be empty)
    build_operation = sys.argv[2]
    build_project = sys.argv[3]
    build_version = sys.argv[4]
    build_subver = sys.argv[5]
    build_qinf = sys.argv[6]
    build_certver = sys.argv[7]
    build_svn = sys.argv[8]
    build_modemapp = sys.argv[9]
    build_oem = sys.argv[10] if len(sys.argv) > 10 else ''

    # Write buildopt.ini to SDK root for compatibility with buildlib_unirtos.bat.
    ini_path = os.path.join(SDK_ROOT_DIR, 'buildopt.ini')

    with open(ini_path, 'w', encoding='utf-8') as f:
        f.write('[buildopt]\n')
        f.write(f'build_operation={build_operation}\n')
        f.write(f'build_project={build_project}\n')
        f.write(f'build_version={build_version}\n')
        f.write(f'build_subver={build_subver}\n')
        f.write(f'build_qinf={build_qinf}\n')
        f.write(f'build_certver={build_certver}\n')
        f.write(f'build_svn={build_svn}\n')
        f.write(f'build_modemapp={build_modemapp}\n')
        f.write(f'build_oem={build_oem}\n')
        f.write('[end]\n')

    log_info(f'parm write {ini_path}')
    return {
        'build_operation': build_operation,
        'build_project': build_project,
        'build_version': build_version,
        'build_subver': build_subver,
        'build_qinf': build_qinf,
        'build_certver': build_certver,
        'build_svn': build_svn,
        'build_modemapp': build_modemapp,
        'build_oem': build_oem
    }



def write_plat_unirtos_root_path(plat):
    # 1. Check if the passed path exists and is a directory
    if not os.path.exists(plat):
        raise FileNotFoundError(f'Directory does not exist: {plat}')
    if not os.path.isdir(plat):
        raise NotADirectoryError(f'Path is not a directory: {plat}')

    # 2. Construct JSON file path.
    json_path = os.path.join(plat, 'sdk_update_config.json')

    # Build JSON data
    config_data = {
        'SDK_PATH': SDK_ROOT_DIR
    }

    # 3. Write JSON file
    with open(json_path, 'w', encoding='utf-8') as f:
        json.dump(config_data, f, indent=4, ensure_ascii=False)

    log_info(f'unirtos root write success: {json_path}')



def prepare_proc():
    # 1. Write buildopt.ini file locally
    config_data = write_buildopt_ini()
    log_info(config_data['build_project'])

    # 2. Check if the corresponding board can be found in platform.json
    board_platform = get_plat_by_board(config_data['build_project'])
    if not board_platform:
        error_proc('at platform.json not find project corresponding platform info', 'CommandError', 'EBOAT')
        help_proc()
        return

    log_info('=======================================')
    log_warning('========prepare success================')
    log_info('=======================================')


# main entry
if __name__ == '__main__':
    if argc == 2:
        help_proc()
    elif argc == 3:
        if argv[2] == 'help':
            help_proc()
        elif argv[1] == 'boards':
            boards = get_plat_by_board(argv[2])
            # Set temporary environment variable
            os.environ['TEMP_VAR'] = boards
            # Output environment variable value to standard output
            print(boards)
        else:
            error_proc(argv[2])
    else:
        # Check number of command line arguments, at least 10 required arguments
        if len(sys.argv) < 11:
            log_warning(f'argv1 %s', sys.argv[1])
            log_warning(f'argv2 %s', sys.argv[2])
            log_warning(f'argv10 %s', sys.argv[9])
            log_warning('used: unirtos prepare plat operation project version subver qinf certver svn modemapp [oem]')
            sys.exit(1)

        if argv[1] == 'prepare':
            prepare_proc()
        else:
            error_proc(argv[1])
