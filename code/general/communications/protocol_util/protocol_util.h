#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void encode_buffer(uint8_t *buffer, int len);

void decode_buffer(uint8_t *buffer, int len);
