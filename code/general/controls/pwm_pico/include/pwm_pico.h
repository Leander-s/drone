#include <hardware/clocks.h>
#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <hardware/structs/io_bank0.h>
#include <pico.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>
#include <pico/time.h>

void init_pwm(int pin);
void set_pwm(int pin, float level);
