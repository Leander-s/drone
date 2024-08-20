#include "util.h"

void disable_terminal_input(struct termios *original){
    struct termios new_settings;

    tcgetattr(STDIN_FILENO, original); 

    new_settings = *original;

    new_settings.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
}

void restore_terminal_input(struct termios *original){
    tcflush(STDIN_FILENO, TCIFLUSH);
    tcsetattr(STDIN_FILENO, TCSANOW, original);
}
