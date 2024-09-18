#include "debugging_config.h"

int pico_read(uint8_t *readBuffer, int size);

int pico_print(uint8_t *message, int len);

void pico_debug_print(char *message, int len);
