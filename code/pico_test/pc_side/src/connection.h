#include "config.h"

typedef struct {
  int connection;
  char *readBuffer;
  char *sendBuffer;
} master;

int initConnection(const char *path);
