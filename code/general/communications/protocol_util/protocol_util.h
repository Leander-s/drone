#pragma once
#include <stdint.h>

typedef union {
    int i;
    uint8_t bytes[4];
}IntBytes;

void encode_buffer(uint8_t *buffer, int len);

void decode_buffer(uint8_t *buffer, int len);
