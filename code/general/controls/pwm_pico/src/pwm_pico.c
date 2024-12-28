#include <pwm_pico.h>

void init_throttle(int pin) {
  gpio_set_function(pin, GPIO_FUNC_PWM);

  // Get the PWM slice and channel associated with the GPIO
  uint slice_num = pwm_gpio_to_slice_num(pin);

  uint16_t wrap = 62500;

  pwm_set_wrap(slice_num, wrap);
  pwm_set_clkdiv(slice_num, 40);

  // Enable PWM output
  pwm_set_enabled(slice_num, true);

  set_throttle(pin, 0.0f);
  sleep_ms(5000);
}

void set_throttle(int pin, float level) {
  uint16_t minPulse = 3125;
  uint16_t maxPulse = 6250;

  uint16_t pulseWidth = (uint16_t)(minPulse + level * (maxPulse - minPulse));

  pwm_set_gpio_level(pin, pulseWidth);
}

void init_servo(int pin) {
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

void set_angle(int pin, float angle) {
  float cycle = (angle / 180) * 1.5f + 0.5f;

  uint16_t value = (uint16_t)(cycle / 20.0f * 39062);

  pwm_set_chan_level(pwm_gpio_to_slice_num(pin), pwm_gpio_to_channel(pin),
                     value);
}
