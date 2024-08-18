#include "nrf24.h"

const uint LED_PIN = 25;

void setup() {
  stdio_usb_init();

  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }

  while (!tud_cdc_connected()) {
    sleep_ms(100);
  }

  pico_print("Pico connected");

  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  gpio_put(LED_PIN, 0);

  nrf24_init();
  nrf24_setup();
}

void loop() {
  while (1) {
    uint8_t data[128];
    memset(data, 0, 128);
    nrf24_read(data, 128);
    if (data[0] != 0) {
      gpio_put(25, 1);
      pico_print("Received data");
    } else {
      gpio_put(25, 0);
    }
    nrf24_flush_rx();
  }
}

int main() {
  setup();
  loop();
  return 0;
}
