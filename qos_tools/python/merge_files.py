#!/usr/bin/env python3
import os
import sys

def merge_files(file1, file2, output):
    """
    Merge file1 and file2 into the output file.
    If file1 or file2 has the same name as the output file, first read the corresponding content into memory,
    to prevent overwriting the original data when writing.
    If a file does not exist, skip that file.
    """
    # Get absolute paths of files for easier comparison
    abs_file1 = os.path.abspath(file1)
    abs_file2 = os.path.abspath(file2)
    abs_output = os.path.abspath(output)

    # If input files have the same name as output file, read content in advance
    content1 = None
    if os.path.exists(file1) and abs_file1 == abs_output:
        with open(file1, 'rb') as f:
            content1 = f.read()

    content2 = None
    if os.path.exists(file2) and abs_file2 == abs_output:
        with open(file2, 'rb') as f:
            content2 = f.read()

    # Open output file, using binary write mode (overwrite if exists)
    with open(output, 'wb') as out:
        # Write content of file1
        if os.path.exists(file1):
            if content1 is not None:
                out.write(content1)
            else:
                with open(file1, 'rb') as f:
                    while True:
                        chunk = f.read(4096)
                        if not chunk:
                            break
                        out.write(chunk)
        else:
             print(f"[merge_files] Warning: File '{file1}' does not exist, skipping.")

        # Write content of file2
        if os.path.exists(file2):
            if content2 is not None:
                out.write(content2)
            else:
                with open(file2, 'rb') as f:
                    while True:
                        chunk = f.read(4096)
                        if not chunk:
                            break
                        out.write(chunk)
        else:
            print(f"[merge_files] Warning: File '{file2}' does not exist, skipping.")

if __name__ == '__main__':
    if len(sys.argv) != 4:
        print("Actual arguments:", sys.argv)
        print("Usage: python merge_files.py file1 file2 file3")
        sys.exit(1)

    file1 = sys.argv[1]
    file2 = sys.argv[2]
    output = sys.argv[3]

    merge_files(file1, file2, output)
    print(f"[merge_files] Merge completed, output file is: {output}")
