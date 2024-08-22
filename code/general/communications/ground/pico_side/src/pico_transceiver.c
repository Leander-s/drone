#include <debugging_util.h>
#include <nrf24.h>
#include <pico/time.h>

#define READ_TIMEOUT_US 10000

void setup() {
  stdio_usb_init();

  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }

  while (!tud_cdc_connected()) {
    sleep_ms(100);
  }

  pico_debug_print("Pico connected", 32);

  nrf24_init();
}

void loop() {
  char message[32];
  memset(message, 0, 32);
  int result;
  while (1) {
    result = pico_read(message, 32);
    if (result < 32) {
      continue;
    }
    if ((message[31] - 1) & 1) {
      int blink = 0;
      while (!stdio_usb_connected()) {
        gpio_put(LED_PIN, blink);
        blink = (blink + 1) % 2;
        sleep_ms(100);
      }
      while (!tud_cdc_connected()) {
        gpio_put(LED_PIN, blink);
        blink = (blink + 1) % 2;
        sleep_ms(100);
      }
      pico_debug_print("Pico connected", 32);
      continue;
    }
    nrf24_send((uint8_t *)message, 32);
    nrf24_read((uint8_t *)message, 32, READ_TIMEOUT_US);
    pico_print(message, 32);
  }
}

int main() {
  setup();
  loop();
  return 0;
}
