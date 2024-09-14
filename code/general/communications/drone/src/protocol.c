#include "protocol.h"
#include "protocol_util.h"
#include <pico/time.h>

DroneTransceiver *drone_protocol_init(DroneTransceiverCreateInfo *createInfo) {
#ifdef NDEBUG
#else
    stdio_usb_init();
    
    while(!stdio_usb_connected()){
        sleep_ms(100);
    }

    while(!tud_cdc_connected()){
        sleep_ms(100);
    }

    LOG("Pico connected", 32);
#endif

  createInfo->init();

  DroneTransceiver *result = malloc(sizeof(DroneTransceiver));
  result->currentState = (DroneState){};
  result->bufferSize = createInfo->bufferSize;
  result->readBuffer = malloc(result->bufferSize);
  result->sendBuffer = malloc(result->bufferSize);
  result->send = createInfo->send;
  result->recv = createInfo->recv;

  drone_flush_rx(result);
  drone_flush_tx(result);

  return result;
}

void drone_protocol_run(DroneTransceiver *transceiver) {
  while (1) {
    drone_read(transceiver);
    drone_protocol_handle_message(transceiver);
    // wait for ground to be ready
    drone_send(transceiver);
  }
  drone_protocol_terminate(transceiver);
}

void drone_protocol_update(DroneTransceiver *transceiver){
    drone_read(transceiver);
    drone_protocol_handle_message(transceiver);
    drone_send(transceiver);
}

// Set actions depending on received message
/*
 * Message layout
 *
 * 1 Byte for everything should be enough
 * 1. throttle
 * 2. pitch
 * 3. roll
 * 4. yaw
 *
 * thats all for now, if we add more, remember to update Drone action and the
 * protocol
 */
void drone_protocol_handle_message(DroneTransceiver *transceiver) {
  uint8_t *message = transceiver->readBuffer;
  decode_buffer(message, 32);
  transceiver->currentState.throttle = message[0];
  transceiver->currentState.pitch = message[1];
  transceiver->currentState.roll = message[2];
  transceiver->currentState.yaw = message[3];

  // flush readBuffer
  drone_flush_rx(transceiver);

  // flush writeBuffer
  drone_flush_tx(transceiver);
  uint8_t *return_message = transceiver->sendBuffer;

  // testing
  memcpy(return_message + 1, "Hello", strlen("Hello"));
  return_message[0] = 1;

  encode_buffer(return_message, 32);
}

void drone_protocol_terminate(DroneTransceiver *transceiver) {
  free(transceiver->readBuffer);
  free(transceiver->sendBuffer);
  free(transceiver);
}

int drone_send(DroneTransceiver *transceiver) {
  return transceiver->send(transceiver->sendBuffer, transceiver->bufferSize);
}

int drone_read(DroneTransceiver *transceiver) {
  return transceiver->recv(transceiver->readBuffer, transceiver->bufferSize, -1);
}

void drone_flush_rx(DroneTransceiver *transceiver) {
  memset(transceiver->readBuffer, 0, transceiver->bufferSize);
}

void drone_flush_tx(DroneTransceiver *transceiver) {
  memset(transceiver->sendBuffer, 0, transceiver->bufferSize);
}
