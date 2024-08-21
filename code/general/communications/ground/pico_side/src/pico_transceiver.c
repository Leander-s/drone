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

  char *message = "\0Pico connected";
  message[0] = 1;
  pico_print(message);

  nrf24_init();
}

void loop() {
  while (1) {
    static char message[32];
    memset(message, 0, 32);
    int result = pico_read(message, 32);
    if (strcmp(message, "") == 0) {
      continue;
    }
    if (strcmp(message, "q") == 0) {
      pico_print("Quitting");
      break;
    }
    if (!tud_cdc_connected()) {
      break;
    }
    nrf24_send((uint8_t*)message, 32);
    memset(message, 0, 32);
    nrf24_read((uint8_t*)message, 32);
    pico_print(message);
    stdio_flush();
  }
}

int main() {
  setup();
  loop();
  return 0;
}
