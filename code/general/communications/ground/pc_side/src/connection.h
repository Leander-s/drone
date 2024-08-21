#include "ground_protocol_config.h"

int initConnection(const char *path);

int pico_write(int port, uint8_t* writeBuffer);

int pico_read(int port, uint8_t* readBuffer, int amount);
