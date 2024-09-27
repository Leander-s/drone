#include <hardware/gpio.h>
#include <hardware/structs/io_bank0.h>
#include <pico.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>
#include <hardware/pwm.h>
#include <pico/time.h>

#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Function to set the PWM frequency and duty cycle
void set_pwm_duty_cycle(uint slice_num, uint channel, float duty_cycle) {
    uint16_t level = (uint16_t)(duty_cycle * 65535);
    pwm_set_gpio_level(slice_num, level);
}

int main() {
    // Initialize the chosen GPIO pin for PWM
    uint gpio_pin = 15;  // GPIO pin connected to the ESC signal
    gpio_set_function(gpio_pin, GPIO_FUNC_PWM);

    // Get the PWM slice and channel associated with the GPIO
    uint slice_num = pwm_gpio_to_slice_num(gpio_pin);

    // Set the PWM frequency (ESC expects 50Hz signal)
    pwm_set_wrap(slice_num, 49999);  // 50Hz (20ms period)

    // Enable PWM output
    pwm_set_enabled(slice_num, true);

    // Step 1: Calibrate the ESC (Max throttle)
    set_pwm_duty_cycle(slice_num, gpio_pin, 0.10f);  // 10% duty cycle (2ms pulse)
    sleep_ms(2000);  // Wait for 2 seconds

    // Step 2: Set to minimum throttle (1ms pulse)
    set_pwm_duty_cycle(slice_num, gpio_pin, 0.05f);  // 5% duty cycle (1ms pulse)
    sleep_ms(2000);  // Wait for 2 seconds

    // Step 3: Arm the ESC and run motor at low throttle
    set_pwm_duty_cycle(slice_num, gpio_pin, 0.06f);  // Slightly higher than min throttle
    sleep_ms(2000);  // Let the motor run

    // Step 4: Run the motor at mid throttle
    set_pwm_duty_cycle(slice_num, gpio_pin, 0.075f);  // 7.5% duty cycle (1.5ms pulse)
    
    // Keep motor running
    while (true) {
        // Continue running motor at a steady speed or add dynamic control here
        sleep_ms(100);
    }
}

