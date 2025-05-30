#include <class/cdc/cdc_device.h>
#include <debugging_util.h>
#include <diagnostics.h>
#include <drone_states.h>
#include <pico/stdio_usb.h>
#include <pico/time.h>
#include <protocol_util.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef NDEBUG
#define LOG(x, y)
#else
#define LOG(x, y) pico_print(x, y)
#endif
