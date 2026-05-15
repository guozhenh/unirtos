import os
import shutil

def get_unirtos_root():
    # Search for unirtos.exe using the system PATH.
    unirtos_path = shutil.which("unirtos.exe")
    if unirtos_path is None:
        return None
    # Get the directory where unirtos.exe is located (e.g., E:\unirtos\bin).
    unirtos_bin_dir = os.path.dirname(unirtos_path)
    # Get the parent directory, i.e., the root directory of Unirtos (e.g., E:\unirtos).
    unirtos_root = os.path.dirname(unirtos_bin_dir)
    return unirtos_root

if __name__ == '__main__':
    root = get_unirtos_root()
    if root:
        print(root)
    else:
        print("not find unirtos.exe")
