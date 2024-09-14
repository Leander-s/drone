#pragma once
#include "ground_protocol_config.h"

#ifdef _WIN32
HANDLE initConnection(const char *path);

int writePort(HANDLE port, uint8_t* writeBuffer, unsigned long toWrite);

int readPort(HANDLE port, uint8_t* readBuffer, unsigned long toRead);
#else
int initConnection(const char *path);

int writePort(int port, uint8_t* writeBuffer, int amount);

int readPort(int port, uint8_t* readBuffer, int amount);
#endif
