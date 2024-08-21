import os
import platform

os.environ['PICO_SDK_PATH'] = '../../../../pico/pico-sdk'


def build():
    if (not os.path.isdir("build")):
        os.mkdir("build")

    os.chdir("build")
    if platform.system() == "Linux":
        os.system("cmake ..")
    if platform.system() == "Windows":
        os.system('cmake -G "MinGW Makefiles" ..')
    os.system("make")


if __name__ == '__main__':
    build()
