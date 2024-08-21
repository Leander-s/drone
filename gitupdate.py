import sys
import os


def main():
    if (len(sys.argv) == 1):
        print("Need a commit message")
        return

    os.system("git add .")
    os.system("git status")

    valid_input = ["y", "ye", "yes", "n", "no"]
    x = input(
        "Do you want to commit these changes? (y)es to continue, (n)o to reset.")
    while not (x in valid_input):
        print("Invalid input")
    if x == "n" or x == "no":
        os.system("git reset")
        return

    os.system(f"git commit -m \"{sys.argv[1]}\"")
    os.system("git push")


if __name__ == '__main__':
    main()
