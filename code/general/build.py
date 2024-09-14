import os
import platform
import sys
import shutil
from pathlib import Path

windows = platform.system() == 'Windows'
linux = platform.system() == 'Linux'

drone_sdk_path = "../../../pico/pico-sdk"

ground_sdk_path = "../" + drone_sdk_path

drone_target_sdk_path = "../../pico/pico-sdk"

ground_target_sdk_path = "../" + drone_target_sdk_path

drone_projects = [
    "./communications/drone"
]

drone_target_project = "./drone"

drone_targets = {
    "src": "./drone",
}


ground_projects = [
    "./communications/ground/pico_side",
    "./communications/ground/pc_side",
    "./gui",
]

ground_target_projects = {
    "pc": "./ground/pc",
    "pico": "./ground/pico",
}

ground_targets = {
    "pico": "./ground/pico",
    "pc": "./ground/pc",
}


def set_pico_sdk_path(path):
    os.environ["PICO_SDK_PATH"] = path


def build_project(root, path, pico_sdk):
    os.chdir(path)
    if not os.path.isdir("build"):
        os.mkdir("build")
    os.chdir("build")

    set_pico_sdk_path(pico_sdk)

    if windows:
        os.system('cmake -G "MinGW Makefiles" ..')
    elif linux:
        os.system('cmake ..')

    os.system('make')
    os.chdir(root)


def rebuild_project(root, path, pico_sdk):
    os.chdir(path)
    if os.path.isdir("build"):
        shutil.rmtree("build")
    build_project(root, os.getcwd(), pico_sdk)


def clean_folder(path):
    for file in Path(path).glob('*'):
        os.remove(file)


def copy_drone_files():
    # remove all the old files
    clean_folder(drone_targets['src'])

    for folder in drone_projects:
        header_files = Path(folder).glob('*.h')
        source_files = Path(folder).glob('*.c')
        files = header_files + source_files
        for file in files:
            if windows:
                os.system(f"copy {file} {
                          drone_targets['include'].replace('/', '\\')}")
            if linux:
                os.system(f"cp {file} {drone_targets['include']}")


def copy_ground_files():
    # remove all the old files
    clean_folder(ground_targets['pico'])
    clean_folder(ground_targets['pc'])

    # copy in the include files
    for folder in ground_projects:
        if "pico" in folder:
            ground_target = 'pico'
        else:
            ground_target = 'pc'
        header_files = Path(folder).glob('*.h')
        source_files = Path(folder).glob('*.c')
        files = header_files + source_files
        for file in files:
            if windows:
                os.system(f"copy {file} {
                    ground_targets[ground_target].replace('/', '\\')}")
            if linux:
                os.system(f"cp {file} {ground_targets[ground_target]}")


def ensure_targets_exist():
    # drone targets
    for key in drone_targets:
        if not os.path.isdir(drone_targets[key]):
            os.mkdir(drone_targets[key])

    # ground targets
    for key in ground_targets:
        if not os.path.isdir(ground_targets[key]):
            os.mkdir(ground_targets[key])


def copy_files():
    copy_ground_files()
    copy_drone_files()


def build_drone(mode):
    root = os.getcwd()

    if mode == "b":
        build_project(root, drone_target_project, drone_target_sdk_path)
    else:
        rebuild_project(root, drone_target_project, drone_target_sdk_path)


def build_ground(mode):
    root = os.getcwd()

    for project in ground_target_projects:
        if mode == "b":
            build_project(
                root, ground_target_projects[project], ground_target_sdk_path)
        else:
            rebuild_project(
                root, ground_target_projects[project], ground_target_sdk_path)


def build(mode):
    build_drone(mode)
    build_ground(mode)


def main():
    build_type = ""
    if len(sys.argv) > 1:
        arg = sys.argv[1]
        if arg[0] == 'b':
            build_type = "b"
        if arg[0] == 'r':
            build_type = "r"
    if build_type == "":
        while build_type != "b" and build_type != "r":
            build_type = input("Type 'b' for build, 'r' for rebuild\n")

    ensure_targets_exist()
    copy_files()
    build(build_type)


if __name__ == '__main__':
    main()
