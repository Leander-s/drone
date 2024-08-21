#include "protocol.h"

GroundTransceiver *
ground_transceiver_create(GroundTransceiverCreateInfo *info) {
  GroundTransceiver *result = malloc(sizeof(GroundTransceiver));
  result->sendBuffer = malloc(info->bufferSize);
  result->recvBuffer = malloc(info->bufferSize);
  result->port = initConnection(info->path_to_port);
  result->bufferSize = info->bufferSize;
  result->currentState = (DroneState){};
  return result;
}

void ground_transceiver_run(GroundTransceiver *transceiver) {
  // input handler initialized here because it should not be in this file
  // forever
  inputHandler *input = createInputHandler();
  while (1) {
    DroneState state = transceiver->currentState;

    // inputs should be done somewhere else in future
    state.pitch = 127;
    state.roll = 127;
    state.yaw = 127;
    state.throttle = state.throttle;
    if (key_down(input, XK_W))
      state.pitch = 0;
    if (key_down(input, XK_S))
      state.pitch = 255;
    if (key_down(input, XK_A))
      state.roll = 0;
    if (key_down(input, XK_D))
      state.roll = 255;
    if (key_down(input, XK_Q))
      state.yaw = 0;
    if (key_down(input, XK_E))
      state.yaw = 255;
    if (key_down(input, XK_Shift_L))
      state.throttle += 1;
    if (key_down(input, XK_Control_L))
      state.throttle -= 1;

    // sending/receiving data
    uint32_t bufferSize = transceiver->bufferSize;

    uint8_t *sendBuffer = transceiver->sendBuffer;
    memset(sendBuffer, 0, transceiver->bufferSize);

    sendBuffer[0] = state.throttle;
    sendBuffer[1] = state.pitch;
    sendBuffer[2] = state.roll;
    sendBuffer[3] = state.yaw;

    ground_transceiver_send(transceiver);

    uint8_t *recvBuffer = transceiver->recvBuffer;
    memset(recvBuffer, 0, transceiver->bufferSize);

    ground_transceiver_read(transceiver);

    if (ground_transceiver_handle_data(transceiver)) {
      break;
    }

#ifdef _WIN32
    Sleep(10);
#else
    usleep(10000);
#endif
  }
  ground_transceiver_destroy(transceiver);
}

int ground_transceiver_handle_data(GroundTransceiver *transceiver) {
  uint8_t *data = transceiver->recvBuffer;
  printf("%s\n", (char *)data);

  if (data[0] == 42) {
    return 1;
  }

  return 0;
}

void ground_transceiver_destroy(GroundTransceiver *transceiver) {
  free(transceiver->sendBuffer);
  free(transceiver->recvBuffer);
  free(transceiver);
}

void ground_transceiver_send(GroundTransceiver *transceiver) {
  pico_write(transceiver->port, transceiver->sendBuffer);
}

void ground_transceiver_read(GroundTransceiver *transceiver) {
  pico_read(transceiver->port, transceiver->recvBuffer,
            transceiver->bufferSize);
}
