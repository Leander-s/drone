#include "protocol.h"

DroneTransceiver *drone_protocol_init(DroneTransceiverCreateInfo *createInfo) {
  createInfo->init();

  DroneTransceiver *result = malloc(sizeof(DroneTransceiver));
  result->action = 0;
  result->bufferSize = createInfo->bufferSize;
  result->readBuffer = malloc(result->bufferSize);
  result->sendBuffer = malloc(result->bufferSize);
  result->send = createInfo->send;
  result->recv = createInfo->recv;

  return result;
}

void drone_protocol_run(DroneTransceiver *transceiver) {
  while (1) {
    drone_read(transceiver);
    if (transceiver->readBuffer[0] == 0)
      continue;
    drone_protocol_handle_message(transceiver);
    drone_send(transceiver);
  }
  drone_protocol_terminate(transceiver);
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
  char *message = transceiver->readBuffer;
  transceiver->action->throttle = message[0];
  transceiver->action->pitch = message[1];
  transceiver->action->roll = message[2];
  transceiver->action->yaw = message[3];

  // flush readBuffer
  drone_flush_rx(transceiver);  

  // flush writeBuffer
  drone_flush_tx(transceiver);
  char *return_message = transceiver->sendBuffer;

  // testing
  memcpy(return_message, (uint8_t*)"Hello", strlen("Hello"));
}

void drone_protocol_terminate(DroneTransceiver *transceiver) {
  free(transceiver->readBuffer);
  free(transceiver->sendBuffer);
  free(transceiver);
}

void drone_send(DroneTransceiver *transceiver) {
  transceiver->send(transceiver->sendBuffer, transceiver->bufferSize);
}

void drone_read(DroneTransceiver *transceiver) {
  transceiver->recv(transceiver->readBuffer, transceiver->bufferSize);
}

void drone_flush_rx(DroneTransceiver *transceiver) {
  memset(transceiver->readBuffer, 0, transceiver->bufferSize);
}

void drone_flush_tx(DroneTransceiver *transceiver) {
  memset(transceiver->sendBuffer, 0, transceiver->bufferSize);
}
