import sys
import re

def process_template(template_file, output_file, config):
    with open(template_file, 'r', encoding='utf-8') as template:
        content = template.read()

    # Step 1: Process rows containing @KEY@
    pattern = re.compile(r'#cmakedefine\s+(\w+)\s+@\s*(\1)\s*@')
    def replace_with_config(match):
        key = match.group(1)
        if key in config:
            value = config[key]
            if value == 'y':
                return f'#define {key}'
            elif value == 'n':
                return f'/* #undef {key} */'
            elif re.match(r'^-?\d+$', value):  # numerical values
                return f'#define {key} {int(value)}'
            else:  # String or other value
                return f'#define {key} {value}'
        else:
            return f'/* #undef {key} */'
    content = pattern.sub(replace_with_config, content)

    # Step 2: Processing ordinary #cmakedefine lines
    lines = content.split('\n')
    new_lines = []
    for line in lines:
        if line.strip().startswith('#cmakedefine'):
            match = re.match(r'#cmakedefine\s+(\w+)(\s+@.*@)?', line)
            if match:
                key = match.group(1)
                if key in config:
                    value = config[key]
                    if value == 'y':
                        new_line = f'#define {key}'
                    elif value == 'n':
                        new_line = f'/* #undef {key} */'
                    elif re.match(r'^-?\d+$', value):
                        new_line = f'#define {key} {value}'
                    else:
                        new_line = f'#define {key} {value}'
                    new_lines.append(new_line)
                else:
                    new_lines.append(f'/* #undef {key} */')
            else:
                new_lines.append(line)
        else:
            new_lines.append(line)
    content = '\n'.join(new_lines)

    # Step 3: Handle unmatched #cmakedefine entries
    content = re.sub(r'#cmakedefine\s+(\w+)', r'/* #undef \1 */', content)

    with open(output_file, 'w', encoding='utf-8') as output:
        output.write(content)

def load_config(config_file):
    config = {}
    with open(config_file, 'r', encoding='utf-8') as config_f:
        for line in config_f:
            line = line.strip()
            if line and not line.startswith('#'):
                if '=' in line:
                    key_part, value_part = line.split('=', 1)
                    key = key_part.strip()
                    value = value_part.strip().strip('\"')  # Remove quotation marks
                    config[key] = value
                else:
                    config[line.strip()] = 'y'
    return config

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: generate_header.py <config_file> <template_file> <output_file>")
        sys.exit(1)

    config_file = sys.argv[1]
    template_file = sys.argv[2]
    output_file = sys.argv[3]

    config = load_config(config_file)
    process_template(template_file, output_file, config)
