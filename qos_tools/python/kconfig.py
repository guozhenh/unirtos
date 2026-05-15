#!/usr/bin/env python3

import sys

def generate_cmake_config(config_file_path, cmake_config_file_path):
    with open(config_file_path, 'r') as config_file, open(cmake_config_file_path, 'w') as cmake_config_file:
        config_lines = config_file.readlines()

        cmake_config_file.write("# Automatically generated CMake configuration\n\n")

        for line in config_lines:
            line = line.strip()
            if line.startswith("#") or not line:
                continue

            option_name, option_value = line.split("=")
            option_name = option_name.strip()
            option_value = option_value.strip()

            if option_value == 'y':
                cmake_config_file.write("set({}_ENABLED ON CACHE BOOL \"\")\n".format(option_name))
            elif option_value == 'n':
                cmake_config_file.write("set({}_ENABLED OFF CACHE BOOL \"\")\n".format(option_name))
            else:
                cmake_config_file.write("# Unsupported option value: {}={}\n".format(option_name, option_value))

        cmake_config_file.write("\n")

def main():
    print(sys.argv[1]);
    print(sys.argv[2]);
    generate_cmake_config(sys.argv[1], sys.argv[2])

if __name__ == "__main__":
    main()
