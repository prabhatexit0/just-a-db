import os
import sys
import glob

BUILD_WORKSPACE_DIRECTORY = "BUILD_WORKSPACE_DIRECTORY"

def execute():
    if not BUILD_WORKSPACE_DIRECTORY in os.environ:
        raise Exception(f"{BUILD_WORKSPACE_DIRECTORY} not defined")

    build_workspace_dir = os.environ[BUILD_WORKSPACE_DIRECTORY]

    files = []
    for file in glob.glob(build_workspace_dir + "/justadb/**/*.*", recursive=True):
        if file.endswith(".cpp") or file.endswith(".h"):
            files.append(file)

    for file in files:
        filename = file.strip()
        full_path = os.path.join(build_workspace_dir, filename)
        print(f"Running clang-format for: {filename}")
        os.system(f"clang-format -style=file -i {full_path}")

if __name__ == "__main__":
    execute()