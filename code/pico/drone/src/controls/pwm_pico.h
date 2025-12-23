#include <hardware/clocks.h>
#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <hardware/structs/io_bank0.h>
#include <pico.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>
#include <pico/time.h>

void init_throttle(int pin);
void set_throttle(int pin, float level);

void init_servo(int pin);
void set_angle(int pin, float angle);
