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
        os.chdir("build")
        os.system('cmake .. -G "MinGW Makefiles" && make')

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
