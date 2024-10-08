#include <hardware/clocks.h>
#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <hardware/structs/io_bank0.h>
#include <pico.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>
#include <pico/time.h>

// Funtion to init pwm
void init_pwm(uint pin) {
  gpio_set_function(pin, GPIO_FUNC_PWM);

  // Get the PWM slice and channel associated with the GPIO
  uint slice_num = pwm_gpio_to_slice_num(pin);

  uint32_t f_sys = clock_get_hz(clk_sys);
  uint32_t pwm_freq = 50;
  uint16_t wrap = 62500;

  pwm_set_wrap(slice_num, wrap);
  pwm_set_clkdiv(slice_num, 40);

  // Enable PWM output
  pwm_set_enabled(slice_num, true);
}

// Function to set the PWM frequency and duty cycle
void set_pwm_duty_cycle(uint pin, float power) {
  uint16_t minPulse = 3125;
  uint16_t maxPulse = 6250;

  uint16_t pulseWidth = (uint16_t)(minPulse + power * (maxPulse - minPulse));

  pwm_set_gpio_level(pin, pulseWidth);
}

int main() {
  // init LED
  gpio_init(25);
  gpio_set_dir(25, GPIO_OUT);
  gpio_put(25, 0);

  // Initialize the chosen GPIO pin for PWM
  uint gpio_pin = 1; // GPIO pin connected to the ESC signal
  init_pwm(gpio_pin);

  // throttle all the way down
  gpio_put(25, 1);
  set_pwm_duty_cycle(gpio_pin, 0.0f);
  sleep_ms(5000); // Wait for 5 seconds
  gpio_put(25, 0);

  for (float i = 0; i < 1.0f; i += 0.1f) {
    set_pwm_duty_cycle(gpio_pin, i);
    gpio_put(25, 1);
    sleep_ms(100);
    gpio_put(25, 0);
    sleep_ms(1000);
  }

  gpio_put(25, 1);
  sleep_ms(100);
  gpio_put(25, 0);
  set_pwm_duty_cycle(gpio_pin, 1.0f);
  sleep_ms(1000);

  for (float i = 0.9f; i > 0.0f; i -= 0.1f) {
    gpio_put(25, 1);
    sleep_ms(100);
    gpio_put(25, 0);
    set_pwm_duty_cycle(gpio_pin, i);
    sleep_ms(1000);
  }

  set_pwm_duty_cycle(gpio_pin, 0.0f);

  /*
   * manually trying to run it
   */
  /*
  gpio_init(1);
  gpio_init(25);
  gpio_set_dir(1, GPIO_OUT);
  gpio_set_dir(25, GPIO_OUT);
  gpio_put(1, 0);
  gpio_put(25, 1);
  int time = 20000;
  int cycle = 2000;
  int calibrated = 0;
  int count = 0;

  sleep_ms(500);

  while (!calibrated) {
    gpio_put(1, 1);
    sleep_us(2000);
    gpio_put(1, 0);
    sleep_us(18000);
    count++;
    if (count == 250) {
      break;
    }
  }

  calibrated = 0;
  count = 0;

  while (!calibrated) {
    gpio_put(1, 1);
    sleep_us(1000);
    gpio_put(1, 0);
    sleep_us(19000);
    count++;
    if (count == 250) {
      break;
    }
  }

  sleep_ms(500);

  gpio_put(25, 1);

  for (int i = 0; i <= 20; i++) {
    gpio_put(1, 1);
    sleep_us(cycle / 20 * i);
    gpio_put(1, 0);
    sleep_us(time - (cycle / 20 * i));
  }

  for (int i = 0; i < 50; i++) {
    gpio_put(1, 1);
    sleep_us(cycle / 20 * 20);
    gpio_put(1, 0);
    sleep_us(time - (cycle / 20 * 20));
  }

  for (int i = 20; i >= 0; i--) {
    gpio_put(1, 1);
    sleep_us(cycle / 20 * i);
    gpio_put(1, 0);
    sleep_us(time - (cycle / 20 * i));
  }
  */

  return 0;
}
