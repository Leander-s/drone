import os
import platform


def build():
    if not os.path.isdir("build"):
        os.mkdir("build")
    os.chdir("build")

    if platform.system() == "Windows":
        os.system('cmake -G "MinGW Makefiles" ..')
    elif platform.system() == "Linux":
        os.system('cmake ..')

    os.system("make")


if __name__ == '__main__':
    build()
