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
  float cycle = (angle / 180.0f) * 2.0f + 0.5f;

  uint16_t value = (uint16_t)(cycle/20.0f * 39062);

  pwm_set_chan_level(pwm_gpio_to_slice_num(pin), pwm_gpio_to_channel(pin),
                     value);
}

float set_for(int pin, int pitch, uint32_t time_us){
  int done = 0;
  uint32_t start = time_us_32();
  while(!done){
      float angle = (float)(127 + pitch) / 255 * 180;
      set_angle(pin, angle);
      uint32_t current = time_us_32();
      if(current - start > time_us){
          done = 1;
      }
  }
}

float set_one_sec(int pin, int pitch){
  int done = 0;
  uint32_t start = time_us_32();
  while(!done){
      float angle = (float)(127 + pitch) / 255 * 180;
      set_angle(pin, angle);
      uint32_t current = time_us_32();
      if(current - start > 1000000){
          done = 1;
      }
  }
}

int main() {
  // init LED
  gpio_init(25);
  gpio_set_dir(25, GPIO_OUT);
  gpio_put(25, 0);

  // Initialize the chosen GPIO pin for PWM
  uint gpioPin1 = 3; // GPIO pin connected to the ESC signal
                    
  init_pwm(gpioPin1);

  set_angle(gpioPin1, 180.0f);
  sleep_ms(2000);

  set_angle(gpioPin1, 0.0f);
  sleep_ms(2000);

  set_one_sec(gpioPin1, 0);
  for(int i = -127; i < 0; i++){
      set_for(gpioPin1, i, 100000);
  }
  for(int i = 0; i < 127; i++){
      set_for(gpioPin1, i, 100000);
  }

  return 0;
}
