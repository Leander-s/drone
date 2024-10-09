#include <hardware/clocks.h>
#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <hardware/structs/io_bank0.h>
#include <pico.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>
#include <pico/time.h>

#define WRAP 39062

// Funtion to init pwm
void init_pwm(uint pin) {
  gpio_set_function(pin, GPIO_FUNC_PWM);

  // Get the PWM slice and channel associated with the GPIO
  uint slice_num = pwm_gpio_to_slice_num(pin);

  uint32_t f_sys = clock_get_hz(clk_sys);
  uint32_t pwm_freq = 50;
  uint16_t wrap = 39062;

  pwm_set_wrap(slice_num, wrap);
  pwm_set_clkdiv(slice_num, 64);

  // Enable PWM output
  pwm_set_enabled(slice_num, true);
}

// Function to set the PWM frequency and duty cycle
void set_angle(int pin, float angle) {
  float cycle = (angle / 180) * 5.0f + 5.0f;

  uint16_t value = (uint16_t)(cycle/100.0f * WRAP);

  pwm_set_chan_level(pwm_gpio_to_slice_num(pin), pwm_gpio_to_channel(pin),
                     value);
}

int main() {
  // init LED
  gpio_init(25);
  gpio_set_dir(25, GPIO_OUT);
  gpio_put(25, 0);

  // Initialize the chosen GPIO pin for PWM
  uint gpio_pin = 0; // GPIO pin connected to the ESC signal
  init_pwm(gpio_pin);

  set_angle(gpio_pin, 180.0f);
  sleep_ms(5000);

  set_angle(gpio_pin, 0.0f);

  return 0;
}
