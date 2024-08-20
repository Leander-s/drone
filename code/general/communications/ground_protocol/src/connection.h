#include "ground_protocol_config.h"

typedef struct {
  int connection;
  char *readBuffer;
  char *sendBuffer;
} master;

int initConnection(const char *path);

int pico_write(int port, char* writeBuffer);

int pico_read(int port, char* readBuffer, int amount);
