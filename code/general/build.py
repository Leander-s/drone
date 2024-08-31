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
    "./communications/ground/pico_side",
    "./communications/ground/pc_side",
    "./gui",
]

ground_target_projects = {
    "pc": "./ground/pc",
    "pico": "./ground/pico",
}

ground_includes = [
    "./communications/ground/pc_side/src",
    "./communications/ground/pico_side/src",
    "./gui/src",
]

ground_libs = [
    "./communications/ground/pc_side/build",
    "./communications/ground/pico_side/build",
    "./gui/build",
]

ground_targets = {
    "pico_lib": "./ground/pico/lib",
    "pc_lib": "./ground/pc/lib",
    "pico_include": "./ground/pico/include",
    "pc_include": "./ground/pc/include",
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
    clean_folder(drone_targets['include'])
    clean_folder(drone_targets['lib'])

    for folder in drone_includes:
        files = Path(folder).glob('*.h')
        for file in files:
            if windows:
                os.system(f"copy {file} {
                          drone_targets['include'].replace('/', '\\')}")
            if linux:
                os.system(f"cp {file} {drone_targets['include']}")

    for build in drone_libs:
        files = Path(build).glob('*.a')
        for file in files:
            if windows:
                os.system(f"copy {file} {
                          drone_targets['lib'].replace('/', '\\')}")
            if linux:
                os.system(f"cp {file} {drone_targets['lib']}")


def copy_ground_files():
    # remove all the old files
    clean_folder(ground_targets['pico_lib'])
    clean_folder(ground_targets['pc_lib'])
    clean_folder(ground_targets['pico_include'])
    clean_folder(ground_targets['pc_include'])

    # copy in the include files
    for folder in ground_includes:
        if "pico" in folder:
            ground_target = 'pico_include'
        else:
            ground_target = 'pc_include'
        files = Path(folder).glob('*.h')
        for file in files:
            if windows:
                os.system(f"copy {file} {
                    ground_targets[ground_target].replace('/', '\\')}")
            if linux:
                os.system(f"cp {file} {ground_targets[ground_target]}")

    for build in ground_libs:
        if "pico" in build:
            ground_target = 'pico_lib'
            files = Path(build).glob('*.uf2')
        else:
            ground_target = 'pc_lib'
            files = Path(build).glob('*.a')
        for file in files:
            if windows:
                os.system(f"copy {file} {
                          ground_targets[ground_target].replace('/', '\\')}")
            if linux:
                os.system(f"cp {file} {
                          ground_targets[ground_target]}")


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


def build_drone_files():
    root = os.getcwd()
    for project_folder in drone_projects:
        build_project(root, project_folder, drone_sdk_path)


def rebuild_drone_files():
    root = os.getcwd()
    for project_folder in drone_projects:
        rebuild_project(root, project_folder, drone_sdk_path)


def build_ground_files():
    root = os.getcwd()
    for project_folder in ground_projects:
        build_project(root, project_folder, ground_sdk_path)


def rebuild_ground_files():
    root = os.getcwd()
    for project_folder in ground_projects:
        build_project(root, project_folder, ground_sdk_path)


def rebuild_files():
    rebuild_ground_files()
    rebuild_drone_files()


def build_files():
    build_ground_files()
    build_drone_files()


def finish_drone_build(mode):
    root = os.getcwd()

    if mode == "b":
        build_project(root, drone_target_project, drone_target_sdk_path)
    else:
        rebuild_project(root, drone_target_project, drone_target_sdk_path)


def finish_ground_build(mode):
    root = os.getcwd()

    for project in ground_target_projects:
        if mode == "b":
            build_project(
                root, ground_target_projects[project], ground_target_sdk_path)
        else:
            rebuild_project(
                root, ground_target_projects[project], ground_target_sdk_path)


def finish_build(mode):
    finish_drone_build(mode)
    finish_ground_build(mode)


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

    ensure_targets_exist()
    copy_files()
    finish_build(build_type)


if __name__ == '__main__':
    main()
