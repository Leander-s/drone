import os
import build

# delete build
if (os.path.isdir("build")):
    os.rmdir("build")
os.chdir("pc_side")
if (os.path.isdir("build")):
    os.rmdir("build")
os.chdir("../pico_side")
if (os.path.isdir("build")):
    os.rmdir("build")
os.chdir("..")

build.build()
