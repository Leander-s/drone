#include <stdio.h>
#include <string.h>
#include <hardware/gpio.h>
#include "class/cdc/cdc_device.h"
#include <pico/binary_info/code.h>
#include <pico/stdio.h>
#include <tusb.h>

const uint LED_PIN = 25;

void setup() {
  stdio_init_all();

  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  while(!tud_cdc_connected()){
      sleep_ms(100);
  }
  stdio_puts("Pico connected\n");
  gpio_put(LED_PIN, 0);
}

void loop() {
  while (1) {
      static char message[128];
      stdio_get_until(message, 1, 50);
      if(strcmp(message, "q") == 0){
        break;
      }
      if(!tud_cdc_connected()){
        break;
      }
      if(strcmp(message, "") != 0){
        gpio_put(LED_PIN, 1);
      }else{
        gpio_put(LED_PIN, 0);
      }
      memset(message, '\0', 128);
  }
}

int main() {
    setup();
    loop();
}
