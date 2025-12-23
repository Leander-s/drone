#include "diagnostics.h"
#include <pico/time.h>
#include <protocol.h>

DroneTransceiver *drone_protocol_init(DroneTransceiverCreateInfo *createInfo) {
#ifdef NDEBUG
#else
  stdio_usb_init();

  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }

  while (!tud_cdc_connected()) {
    sleep_ms(100);
  }

  LOG("Pico connected", 32);
#endif

  createInfo->init();

  DroneTransceiver *result =
      (DroneTransceiver *)malloc(sizeof(DroneTransceiver));
  result->systemLog = createInfo->log;
  result->sensorState = createInfo->sensorState;
  result->controlState = createInfo->controlState;
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
    drone_protocol_prepare_data(transceiver);
    drone_read(transceiver);
    drone_send(transceiver);
    drone_protocol_handle_message(transceiver);
  }
  drone_protocol_terminate(transceiver);
}

void drone_protocol_update(DroneTransceiver *transceiver) {
  drone_protocol_prepare_data(transceiver);
  int readBytes = drone_read(transceiver);
  int sentBytes = drone_send(transceiver);
  if (readBytes == 32) {
    drone_protocol_handle_message(transceiver);
  }
}

void drone_protocol_prepare_data(DroneTransceiver *transceiver) {
  // flush writeBuffer
  drone_flush_tx(transceiver);
  uint8_t *message = transceiver->sendBuffer;
  int offset = 0;

  // Sending back sensorState
  message[0] = 3;
  offset++;

  memcpy(message + 1, transceiver->sensorState->bytes, 8);
  offset += 8;

  // Sending free memory
  /*
  // finding free memory
  int blockSize = 4096;
  int allocSize = blockSize;
  uint8_t *ptr = malloc(allocSize);
  while (ptr != NULL) {
    free(ptr);
    allocSize += blockSize;
    ptr = malloc(allocSize);
  }
  free(ptr);
  IntBytes freeMem;
  freeMem.i = allocSize - blockSize;
  memcpy(return_message + offset, freeMem.bytes, 4), offset += 4;
  */

  encode_buffer(message, 32);
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
  // As of right now, this just sets controls to a certain state. Eventually
  // I want this to change a target quaternion, that controls can approximate
  // automatically, but this is easier for now, without controls being
  // implemented
  transceiver->controlState->throttle = message[0];
  transceiver->controlState->pitch = message[1];
  transceiver->controlState->roll = message[2];
  transceiver->controlState->yaw = message[3];

  // flush readBuffer
  drone_flush_rx(transceiver);
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
  return transceiver->recv(transceiver->readBuffer, transceiver->bufferSize,
                           RADIO_READ_TIMEOUT);
}

void drone_flush_rx(DroneTransceiver *transceiver) {
  memset(transceiver->readBuffer, 0, transceiver->bufferSize);
}

void drone_flush_tx(DroneTransceiver *transceiver) {
  memset(transceiver->sendBuffer, 0, transceiver->bufferSize);
}
