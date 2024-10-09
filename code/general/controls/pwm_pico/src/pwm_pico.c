#include <pwm_pico.h>

void init_pwm(int pin){
  gpio_set_function(pin, GPIO_FUNC_PWM);

  // Get the PWM slice and channel associated with the GPIO
  uint slice_num = pwm_gpio_to_slice_num(pin);

  uint16_t wrap = 62500;

  pwm_set_wrap(slice_num, wrap);
  pwm_set_clkdiv(slice_num, 40);

  // Enable PWM output
  pwm_set_enabled(slice_num, true);

  set_pwm(pin, 0.0f);
  sleep_ms(5000);
}

void set_pwm(int pin, float level){
  uint16_t minPulse = 3125;
  uint16_t maxPulse = 6250;

  uint16_t pulseWidth = (uint16_t)(minPulse + level * (maxPulse - minPulse));

  pwm_set_gpio_level(pin, pulseWidth);
}
