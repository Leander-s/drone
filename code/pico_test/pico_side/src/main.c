#include "nrf24.h"

const uint LED_PIN = 25;

void setup() {
  stdio_usb_init();

  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }

  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  while (!tud_cdc_connected()) {
    sleep_ms(100);
  }
  pico_print("Pico connected");
  gpio_put(LED_PIN, 0);

  nrf24_init();
}

void loop() {
  while (1) {
    static char message[128];
    int result = pico_read(message, 128);
    if (strcmp(message, "") == 0) {
      gpio_put(LED_PIN, 0);
      continue;
    }
    if (strcmp(message, "q") == 0) {
      printf("Quitting");
      break;
    }
    if (!tud_cdc_connected()) {
      break;
    }
    gpio_put(LED_PIN, 1);
    pico_print(message);
    int length = strlen(message);
    nrf24_send((uint8_t*)message, length);
    memset(message, '\0', 128);
    stdio_flush();
  }
}

int main() {
  setup();
  loop();
}
