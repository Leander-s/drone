#include <debugging_util.h>
#include <nrf24.h>

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
  while (1) {
    char message[32];
    memset(message, 0, 32);
    int result = pico_read(message, 32);
    if (result < 32) {
      continue;
    }
    if (!tud_cdc_connected()) {
      break;
    }
    nrf24_send((uint8_t *)message, 32);
    memset(message, 0, 32);
    nrf24_read((uint8_t *)message, 32, READ_TIMEOUT_US);
    pico_print(message, 32);
  }
}

int main() {
  setup();
  loop();
  return 0;
}
