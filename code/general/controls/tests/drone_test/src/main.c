#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <hardware/structs/io_bank0.h>
#include <pico.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>
#include <pico/time.h>

#include "hardware/pwm.h"
#include "pico/stdlib.h"

// Function to set the PWM frequency and duty cycle
void set_pwm_duty_cycle(uint slice_num, float duty_cycle) {
  uint16_t minPulse = 1000;
  uint16_t maxPulse = 2000;

  float pulseWidth = minPulse + duty_cycle * (maxPulse - minPulse);

  uint16_t level = (uint16_t)((pulseWidth / 20000.0f) * 65535.0f);
  pwm_set_gpio_level(slice_num, level);
}

int main() {
  /*
// Initialize the chosen GPIO pin for PWM
gpio_init(25);
gpio_set_dir(25, GPIO_OUT);
gpio_put(25, 0);
uint gpio_pin = 0;  // GPIO pin connected to the ESC signal
gpio_set_function(gpio_pin, GPIO_FUNC_PWM);

// Get the PWM slice and channel associated with the GPIO
uint slice_num = pwm_gpio_to_slice_num(gpio_pin);

// Set the PWM frequency (ESC expects 50Hz signal)
pwm_config config = pwm_get_default_config();
pwm_config_set_clkdiv(&config, 125.0f);
pwm_init(slice_num, &config, true);

// Enable PWM output
pwm_set_enabled(slice_num, true);

// Step 1: Calibrate the ESC (Max throttle)
gpio_put(25, 1);
set_pwm_duty_cycle(slice_num, 1.0f);
sleep_ms(10000);  // Wait for 10 seconds
gpio_put(25, 0);

sleep_ms(1000);

// throttle all the way down
gpio_put(25, 1);
set_pwm_duty_cycle(slice_num, 0.0f);
sleep_ms(10000);  // Wait for 2 seconds
gpio_put(25, 0);

// Step 4: Run the motor at mid throttle
set_pwm_duty_cycle(slice_num, 0.5f);  // 7.5% duty cycle (1.5ms pulse)

// Keep motor running
while (true) {
    sleep_ms(100);
}
*/

  /*
   * manually trying to run it
   */
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
    sleep_us(1500);
    gpio_put(1, 0);
    sleep_us(18500);
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
    sleep_us(18500);
    count++;
    if (count == 250) {
      break;
    }
  }

  sleep_ms(500);

  gpio_put(25, 1);

  for (int i = 0; i < 50; i++) {

    gpio_put(1, 1);
    sleep_us(cycle);
    gpio_put(1, 0);
    sleep_us(time - cycle - 500);
  }

  return 0;
}
