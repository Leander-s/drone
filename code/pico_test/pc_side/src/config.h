#ifdef _WIN32
#include <stdio.h>
#include <string.h>
#include <windows.h>
#else
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#endif

#define BUFFER_SIZE 1024
