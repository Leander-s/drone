#pragma once
#include <stdint.h>

#define BNO055_INIT_ERROR 1

typedef union {
    float f;
    uint8_t bytes[4];
}FloatBytes;

typedef union {
    int i;
    uint8_t bytes[4];
}IntBytes;

typedef struct {
    float transmissionsPerSecond;
    int picoReadTimeouts;
    int usbDisconnects;
    int usbReadErrors;
    int usbWriteErrors;
}PCSystemLog;

typedef struct {
    FloatBytes transmissionsPerSecond;
    IntBytes usbDisconnects;
    IntBytes readTimeouts;
}PicoSystemLog;

typedef struct{
    uint8_t error;
}DroneSystemLog;

void read_float_bytes_f(FloatBytes* fb, float f);

void read_float_bytes_b(FloatBytes* fb, uint8_t bytes[4]);

void read_int_bytes_i(IntBytes* ib, int i);

void read_int_bytes_b(IntBytes* ib, uint8_t bytes[4]);
