import os
import platform
import sys
import shutil
from pathlib import Path

windows = platform.system() == 'Windows'
linux = platform.system() == 'Linux'

drone_target_sdk_path = "../../pico/pico-sdk"

ground_target_sdk_path = "../" + drone_target_sdk_path


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


def build_drone(mode):
    root = os.getcwd()

    if mode == "b":
        build_project(root, "drone", drone_target_sdk_path)
    else:
        rebuild_project(root, "drone", drone_target_sdk_path)


def build_ground(mode):
    root = os.getcwd()

    if mode == "b":
        build_project(
            root, "ground/pc", ground_target_sdk_path)
        build_project(
            root, "ground/pico", ground_target_sdk_path)
    else:
        rebuild_project(
            root, "ground/pc", ground_target_sdk_path)
        rebuild_project(
            root, "ground/pico", ground_target_sdk_path)


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

    build(build_type)


if __name__ == '__main__':
    main()
