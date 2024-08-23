import os
import build
import shutil

if __name__ == '__main__':
    if not os.path.isdir("build"):
        build.build()

    shutil.rmtree("build")
    build.build()
