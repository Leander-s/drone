import os
from sys import platform


def build():
    if (not os.path.isdir("build")):
        os.mkdir("build")

    os.chdir("pc_side")

    if (not os.path.isdir("build")):
        os.mkdir("build")

    os.chdir("build")
    os.system("cmake -DCMAKE_BUILD_MODE=Debug ..")
    if platform != "win32":
        os.system("make")

    os.chdir("../../pico_side")

    if (not os.path.isdir("build")):
        os.mkdir("build")

    os.chdir("build")
    os.system("cmake -DCMAKE_BUILD_MODE=Debug ..")
    if platform != "win32":
        os.system("make")

    os.chdir("../..")

    if (not os.path.isdir("pc_build")):
        os.mkdir("pc_build")
    if (not os.path.isdir("pico_build")):
        os.mkdir("pico_build")

    if platform == "win32":
        os.system("copy ./pc_side/build/libground_protocol_pc.a ./pc_build/")
        os.system("copy ./pc_side/build/ground_protocol_pc_test ./pc_build/")
        os.system("copy ./pico_side/build/libground_protocol_pico.a ./pico_build/")
        os.system("copy ./pico_side/build/ground_protocol_pico_test.uf2 ./pico_build/")
    else:
        os.system("cp ./pc_side/build/libground_protocol_pc.a ./pc_build/")
        os.system("cp ./pc_side/build/ground_protocol_pc_test ./pc_build/")
        os.system("cp ./pico_side/build/libground_protocol_pico.a ./pico_build/")
        os.system("cp ./pico_side/build/ground_protocol_pico_test.uf2 ./pico_build/")


if __name__ == '__main__':
    build()
