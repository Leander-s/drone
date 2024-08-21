#include "ground_protocol_config.h"

#ifdef _WIN32
HANDLE initConnection(const char *path);

int writePort(HANDLE port, uint8_t* writeBuffer);

int readPort(HANDLE port, uint8_t* readBuffer, int amount);
#else
int initConnection(const char *path);

int writePort(int port, uint8_t* writeBuffer);

int readPort(int port, uint8_t* readBuffer, int amount);
#endif
