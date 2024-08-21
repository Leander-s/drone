import os
import build
import shutil

# delete build
root = os.getcwd()
if (os.path.isdir("build")):
    shutil.rmtree("build")
os.chdir("pc_side")
if (os.path.isdir("build")):
    shutil.rmtree("build")
os.chdir(root)
os.chdir("pico_side")
if (os.path.isdir("build")):
    shutil.rmtree("build")
os.chdir(root)

build.build()
