#include <nrf24.h>

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
    char data[32];
    memset(data, 0, 32);
    nrf24_read(data, 32);
    pico_print(data);
  }
}

int main() {
  setup();
  loop();
  return 0;
}
