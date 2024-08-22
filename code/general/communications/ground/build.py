import os
import platform

os.environ['PICO_SDK_PATH'] = '../../../../../pico/pico-sdk'


def build():
    os.chdir("pc_side")

    if (not os.path.isdir("build")):
        os.mkdir("build")

    os.chdir("build")
    if platform.system() == "Linux":
        print("Generating makefiles for linux")
        os.system("cmake ..")
    if platform.system() == "Windows":
        print("Generating makefiles for mingw")
        os.system('cmake -G "MinGW Makefiles" ..')
    os.system("make")

    os.chdir("../../pico_side")

    if (not os.path.isdir("build")):
        os.mkdir("build")

    os.chdir("build")
    if platform.system() == "Linux":
        os.system("cmake ..")
    if platform.system() == "Windows":
        os.system('cmake -G "MinGW Makefiles" ..')
    os.system("make")

    os.chdir("../..")

    if (not os.path.isdir("pc_build")):
        os.mkdir("pc_build")
    if (not os.path.isdir("pico_build")):
        os.mkdir("pico_build")

    if platform.system() == "Windows":
        os.system("copy .\\pc_side\\build\\libground_protocol_pc.a .\\pc_build\\")
        os.system("copy .\\pc_side\\build\\ground_protocol_pc_test.exe .\\pc_build\\")
        os.system(
            "copy .\\pico_side\\build\\ground_protocol_pico.uf2 .\\pico_build\\")
    else:
        os.system("cp ./pc_side/build/libground_protocol_pc.a ./pc_build/")
        os.system("cp ./pc_side/build/ground_protocol_pc_test ./pc_build/")
        os.system(
            "cp ./pico_side/build/ground_protocol_pico.uf2 ./pico_build/")


if __name__ == '__main__':
    build()
