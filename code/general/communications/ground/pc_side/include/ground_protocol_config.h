#pragma once
#ifdef _WIN32
#include <windows.h>
#else
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <libudev.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <diagnostics.h>
#include <protocol_util.h>
#include <drone_states.h>
