import os
import platform
import sys
from pathlib import Path

windows = platform.system() == 'Windows'
linux = platform.system() == 'Linux'

drone_projects = [
    "./communications/drone"
]

drone_includes = [
    "./communications/drone/src"
]

drone_libs = [
    "./communications/drone/build"
]

drone_targets = {
    "include": "./drone/include",
    "lib": "./drone/lib"
}

ground_projects = [
    "./communications/ground"
]

ground_includes = [
    "./communications/ground/pc_side/src",
    "./communications/ground/pico_side/src"
]

ground_libs = [
    "./communications/ground/pc_build",
    "./communications/ground/pico_build"
]

ground_targets = {
    "pico_lib": "./ground/pico/lib",
    "pc_lib": "./ground/pc/lib",
    "pico_include": "./ground/pico/include",
    "pc_include": "./ground/pc/include",
}


def copy_drone_files():
    for folder in drone_includes:
        files = Path(folder).glob('*.h')
        for file in files:
            if windows:
                os.system(f"copy {file} {drone_targets['include']}")
                continue
            if linux:
                os.system(f"cp {file} {drone_targets['include']}")
                continue
    for build in drone_libs:
        files = Path(build).glob('*.a')
        for file in files:
            if windows:
                os.system(f"copy {file} {drone_targets['lib']}")
                continue
            if linux:
                os.system(f"cp {file} {drone_targets['lib']}")
                continue


def copy_ground_files():
    for folder in ground_includes:
        if "pico" in folder:
            ground_target = 'pico_include'
        else:
            ground_target = 'pc_include'
        files = Path(folder).glob('*.h')
        for file in files:
            if windows:
                os.system(f"copy {file} {ground_targets[ground_target]}")
            if linux:
                os.system(f"cp {file} {ground_targets[ground_target]}")

    for build in ground_libs:
        if "pico" in build:
            ground_target = 'pico_lib'
        else:
            ground_target = 'pc_lib'
        files = Path(build).glob('*.a')
        for file in files:
            if windows:
                os.system(f"copy {file} {
                          ground_targets[ground_target]}")
                continue
            if linux:
                os.system(f"cp {file} {
                          ground_targets[ground_target]}")
                continue


def copy_files():
    copy_ground_files()
    copy_drone_files()


def build_drone_files():
    root = os.getcwd()
    for project_folder in drone_projects:
        os.chdir(project_folder)
        os.system("python3 build.py")
        os.chdir(root)


def rebuild_drone_files():
    root = os.getcwd()
    for project_folder in drone_projects:
        os.chdir(project_folder)
        os.system("python3 rebuild.py")
        os.chdir(root)


def build_ground_files():
    root = os.getcwd()
    for project_folder in ground_projects:
        os.chdir(project_folder)
        os.system("python3 build.py")
        os.chdir(root)


def rebuild_ground_files():
    root = os.getcwd()
    for project_folder in ground_projects:
        os.chdir(project_folder)
        os.system("python3 rebuild.py")
        os.chdir(root)


def rebuild_files():
    rebuild_ground_files()
    rebuild_drone_files()


def build_files():
    build_ground_files()
    build_drone_files()


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

    if build_type == "b":
        build_files()
    else:
        rebuild_files()
    copy_files()


if __name__ == '__main__':
    main()
