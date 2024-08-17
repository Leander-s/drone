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
      int result = stdio_get_until(message, 1, 20000);
      if(result == PICO_ERROR_TIMEOUT){
          gpio_put(LED_PIN, 0);
          continue;
      }
      if(strcmp(message, "q") == 0){
        break;
      }
      if(!tud_cdc_connected()){
        break;
      }
      gpio_put(LED_PIN, 1);
      memset(message, '\0', 128);
  }
}

int main() {
    setup();
    loop();
}
