#include "protocol_util.h"
#include <pico_transceiver.h>

void busy_wait_for_connect() {
  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }

  while (!tud_cdc_connected()) {
    sleep_ms(100);
  }

  char handshake[10];
  pico_read(handshake, 10);
  pico_debug_print("Pico connected", 32);
}

PicoTransceiver *pico_transceiver_create() {
  stdio_usb_init();
  busy_wait_for_connect();
  nrf24_init();

  PicoTransceiver *result = (PicoTransceiver *)malloc(sizeof(PicoTransceiver));
  result->log.readTimeouts.i = 0;
  result->log.usbDisconnects.i = 0;
  result->log.transmissionsPerSecond.f = 0;
  return result;
}

void pico_transceiver_update(PicoTransceiver *trans) {
  memset(trans->fromPC, 0, 64);
  memset(trans->toPC, 1, 32);
  memset(trans->toPC + 32, 0, 32);
  int result;
  uint64_t start_time = time_us_64();
  result = pico_read(trans->fromPC, 64);
  tud_cdc_read_flush();
  if (result < 0) {
    trans->log.usbDisconnects.i++;
    busy_wait_for_connect();
    return;
  }
  // completely disregard any incomplete transmissions 
  if (result < 32) {
    return;
  }
  // not sure how this works anymore
  if ((trans->fromPC[36] - 1) & 1) {
    trans->log.usbDisconnects.i++;
    busy_wait_for_connect();
    return;
  }
  decode_buffer(trans->fromPC, 64);
  encode_buffer(trans->fromPC, 32);
  int bytesSent = nrf24_send(trans->fromPC, 32);
  int bytesReceived = nrf24_read(trans->toPC, 32, READ_TIMEOUT);
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
  stdio_flush();
  tud_cdc_write_flush();
}

void pico_transceiver_destroy(PicoTransceiver *trans) { free(trans); }
