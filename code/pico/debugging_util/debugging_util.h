#include <pico/stdio.h>
#include <pico/stdio_usb.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <string.h>

#define LED_PIN 25

int pico_read(char *readBuffer, int size);

int pico_print(char *message, int len);

void pico_debug_print(char *message, int len);
