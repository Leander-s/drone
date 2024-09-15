#include <pico_transceiver.h>

void setup() {
    stdio_usb_init();

    busy_wait_for_connect();

    nrf24_init();
}

void busy_wait_for_connect() {
  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }

  while (!tud_cdc_connected()) {
    sleep_ms(100);
  }

  pico_debug_print("Pico connected", 32);
}

PicoTransceiver *pico_transceiver_create(){
    PicoTransceiver *result = (PicoTransceiver*)malloc(sizeof(PicoTransceiver));
    result->log.readTimeouts.i = 0;
    result->log.usbDisconnects.i = 0;
    result->log.transmissionsPerSecond.f = 0;
    memset(result->fromPC, 0, 64);
    memset(result->toPC, 0, 64);
    return result;
}

void pico_transceiver_update(PicoTransceiver *trans){
    int result;
    uint64_t start_time = time_us_64();
    result = pico_read((char *)trans->fromPC, 64);
    if (result < 0) {
      trans->log.usbDisconnects.i++;
      busy_wait_for_connect();
      return;
    }
    if (result < 32) {
      return;
    }
    if ((trans->fromPC[36] - 1) & 1) {
      trans->log.usbDisconnects.i++;
      busy_wait_for_connect();
      return;
    }
    int bytesSent = nrf24_send(trans->fromPC, 32);
    int bytesReceived = nrf24_read(trans->toPC, 32, READ_TIMEOUT_US);
    if (bytesReceived == 0) {
      trans->log.readTimeouts.i++;
    }
    for (int i = 0; i < 4; i++) {
      trans->toPC[32 + i] = trans->log.usbDisconnects.bytes[i];
    }

    for (int i = 0; i < 4; i++) {
      trans->toPC[36 + i] = trans->log.readTimeouts.bytes[i];
    }

    uint64_t end_time = time_us_64();
    trans->log.transmissionsPerSecond.f =
        (1.0 / (float)(end_time - start_time)) * 1000000;

    for (int i = 0; i < 4; i++) {
      trans->toPC[40 + i] = trans->log.transmissionsPerSecond.bytes[i];
    }

    encode_buffer(trans->toPC + 32, 32);

    pico_print(trans->toPC, 64);
}

void pico_transceiver_destroy(PicoTransceiver *trans){
    free(trans);
}
