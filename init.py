import os
import platform
import sys


def init():
    os.system("git submodule update --init --recursive")
    root = os.getcwd()
    os.chdir("code/general/gui/SDL")

    if platform.system() == "Linux":
        os.system(
            "sudo cmake -S . -B build && sudo cmake --build build && sudo cmake --install build")

    elif platform.system() == "Windows":
        arch = sys.argv[1]
        arch_valid = (arch == "86" or arch == "64")
        if not arch_valid:
            while not arch_valid:
                arch_input = input(
                    "Choose architecture:\n86 for x86, 64 or nothing for x64")
                if arch_input == "" or arch_input == "64":
                    arch = "64"
                elif arch_input == "86":
                    arch = "86"
        if arch == "64":
            os.system("cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build-scripts/cmake-toolchain-mingw64-x86-64.cmake && cmake --build build && cmake --install build")
        else:
            os.system("cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build-scripts/cmake-toolchain-mingw64-i686.cmake && cmake --build build && cmake --install build")

    os.chdir(root)
    os.chdir("./code/general/gui/SDL_ttf")
    if (not os.path.isdir("build")):
        os.mkdir("build")
    os.chdir("build")
    if platform.system() == "Linux":
        os.system("cmake .. && make")
    elif platform.system() == "Windows":
        os.system('cmake -G "MinGW Makefiles" .. && make')


if __name__ == '__main__':
    init()
