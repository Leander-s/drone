#include "util.h"

#ifdef _WIN32
#else
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
#endif

uint8_t min(uint8_t a, uint8_t b){
    if(a > b){
        return b;
    }
    return a;
}

uint8_t max(uint8_t a, uint8_t b){
    if(a > b){
        return a;
    }
    return b;
}

uint8_t clamp(uint8_t x, uint8_t a, uint8_t b){
    return max(a, min(b, x));
}
