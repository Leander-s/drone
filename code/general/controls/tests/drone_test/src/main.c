#include <hardware/clocks.h>
#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <hardware/structs/io_bank0.h>
#include <pico.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>
#include <pico/time.h>

// Function to set the PWM frequency and duty cycle
void set_pwm_duty_cycle(uint slice_num, float duty_cycle) {
  uint16_t minPulse = 1000;
  uint16_t maxPulse = 2000;

  float pulseWidth = minPulse + duty_cycle * (maxPulse - minPulse);

  uint16_t level = (uint16_t)((pulseWidth / 20000.0f) * 65535.0f);
  pwm_set_gpio_level(slice_num, level);
}

int main() {
  // Initialize the chosen GPIO pin for PWM
  gpio_init(25);
  gpio_set_dir(25, GPIO_OUT);
  gpio_put(25, 0);

  uint gpio_pin = 1; // GPIO pin connected to the ESC signal
  gpio_set_function(gpio_pin, GPIO_FUNC_PWM);

  // Get the PWM slice and channel associated with the GPIO
  uint slice_num = pwm_gpio_to_slice_num(gpio_pin);

  uint32_t f_sys = clock_get_hz(clk_sys);
  uint32_t pwm_freq = 50;
  uint16_t wrap = f_sys / pwm_freq - 1;

  pwm_set_wrap(slice_num, wrap);

  // Step 1: Calibrate the ESC (Max throttle)
  gpio_put(25, 1);
  set_pwm_duty_cycle(slice_num, 1.0f);
  sleep_ms(1000); // Wait for 5 seconds
  gpio_put(25, 0);

  // throttle all the way down
  gpio_put(25, 1);
  set_pwm_duty_cycle(slice_num, 0.0f);
  sleep_ms(1000); // Wait for 5 seconds
  gpio_put(25, 0);

  // Enable PWM output
  pwm_set_enabled(slice_num, true);

  // Step 4: Run the motor at mid throttle
  for (int i = 0; i < 0.5f; i += 0.1f) {
    set_pwm_duty_cycle(slice_num, i);
    sleep_ms(100);
  }

  set_pwm_duty_cycle(slice_num, 0.5f); // 50% duty cycle (1.5ms pulse)

  // Keep motor running
  for (int i = 0; i < 1000; i++) {
    sleep_ms(100);
  }
  for (int i = 0; i < 0.5f; i += 0.1f) {
    set_pwm_duty_cycle(slice_num, 0.5f - i);
    sleep_ms(100);
  }

  set_pwm_duty_cycle(slice_num, 0);

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
