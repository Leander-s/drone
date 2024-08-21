#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __NDEBUG
#define LOG(x)
#else
#define LOG(x) pico_print(x)
#endif
