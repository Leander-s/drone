#include <nrf24.h>
#include <debugging_util.h>

void setup() {
  stdio_usb_init();

  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }

  while (!tud_cdc_connected()) {
    sleep_ms(100);
  }

  pico_print("Pico connected");

  nrf24_init();
}

void loop() {
  while (1) {
    static char message[32];
    int result = pico_read(message, 32);
    if (strcmp(message, "") == 0) {
      continue;
    }
    if (strcmp(message, "q") == 0) {
      printf("Quitting");
      break;
    }
    if (!tud_cdc_connected()) {
      break;
    }
    pico_print(message);
    nrf24_send((uint8_t*)message, 32);
    memset(message, 0, 32);
    stdio_flush();
  }
}

int main() {
  setup();
  loop();
  return 0;
}
