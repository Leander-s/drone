#include "config.h"

void disable_terminal_input(struct termios *original);

void restore_terminal_input(struct termios *original);
