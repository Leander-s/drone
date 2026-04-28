#include <pico_transceiver.h>

int main() {
  PicoTransceiver transceiver;
  pico_transceiver_init(&transceiver);

  while (1) {
    pico_transceiver_update(&transceiver);
  }
  return 0;
}
