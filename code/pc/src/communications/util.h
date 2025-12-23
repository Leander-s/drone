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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t min(uint8_t a, uint8_t  b);
uint8_t max(uint8_t  a, uint8_t  b);
uint8_t clamp(uint8_t  x, uint8_t  a, uint8_t  b);
