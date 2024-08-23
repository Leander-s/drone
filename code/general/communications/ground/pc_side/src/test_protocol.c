#include "protocol.h"
#include "input_poll.h"

GroundTransceiver *
ground_transceiver_create(GroundTransceiverCreateInfo *info) {
  GroundTransceiver *result = malloc(sizeof(GroundTransceiver));
  result->sendBuffer = malloc(info->bufferSize);
  result->recvBuffer = malloc(info->bufferSize);
  result->port = initConnection(info->path_to_port);
  result->bufferSize = info->bufferSize;
  *result->controlState =
      (DroneControlState){.throttle = 0, .pitch = 127, .roll = 127, .yaw = 127};
  *result->sensorState = (DroneSensorState){};

  ground_transceiver_read(result);
  printf("%s\n", result->recvBuffer);

  return result;
}

// this is for unit testing and multithreading
// Multithreading would still require a way to synchronize data since the structs
// are shared memory
void ground_transceiver_run(GroundTransceiver *transceiver) {
  printf("Ground transceiver running\n");
  // input handler initialized here because it should not be in this file
  // forever
  // The ground_transceiver_protocol should only serialize the data from the
  // DroneControlState, send it to the drone, and the deserialize the reply
  // into the DroneSensorState
  inputHandler *input = createInputHandler();

  while (1) {
    DroneControlState *controlState = transceiver->controlState;
    DroneSensorState *sensorState = transceiver->sensorState;
    uint32_t bufferSize = transceiver->bufferSize;
    uint8_t *sendBuffer = transceiver->sendBuffer;
    memset(sendBuffer, 1, transceiver->bufferSize);

    // inputs should be done somewhere else in future
    controlState->pitch = 127;
    controlState->roll = 127;
    controlState->yaw = 127;
    controlState->throttle = controlState->throttle;
    if (key_down(input, XK_W))
      controlState->pitch = 0;
    if (key_down(input, XK_S))
      controlState->pitch = 254;
    if (key_down(input, XK_A))
      controlState->roll = 0;
    if (key_down(input, XK_D))
      controlState->roll = 254;
    if (key_down(input, XK_Q))
      controlState->yaw = 0;
    if (key_down(input, XK_E))
      controlState->yaw = 254;
    if (key_down(input, XK_Shift_L))
      controlState->throttle += 1;
    if (key_down(input, XK_Control_L))
      controlState->throttle -= 1;

    controlState->throttle = clamp(controlState->throttle, 0, 254);

    // quit on P
    if (key_down(input, XK_P)) {
      sendBuffer[31] = 2;
      break;
    }

    // sending/receiving data
    // adding 1 everywhere so read() doesnt terminate early
    sendBuffer[0] = 1 + controlState->throttle;
    sendBuffer[1] = 1 + controlState->pitch;
    sendBuffer[2] = 1 + controlState->roll;
    sendBuffer[3] = 1 + controlState->yaw;

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

// This is for use in a main loop instead of multithreaded
void ground_transceiver_update(GroundTransceiver *transceiver) {
  DroneControlState *controlState = transceiver->controlState;
  DroneSensorState *sensorState = transceiver->sensorState;
  uint32_t bufferSize = transceiver->bufferSize;
  uint8_t *sendBuffer = transceiver->sendBuffer;
  memset(sendBuffer, 1, transceiver->bufferSize);

  // sending/receiving data
  // adding 1 everywhere so read() doesnt terminate early
  sendBuffer[0] = 1 + controlState->throttle;
  sendBuffer[1] = 1 + controlState->pitch;
  sendBuffer[2] = 1 + controlState->roll;
  sendBuffer[3] = 1 + controlState->yaw;

  ground_transceiver_send(transceiver);

  uint8_t *recvBuffer = transceiver->recvBuffer;
  memset(recvBuffer, 0, transceiver->bufferSize);

  ground_transceiver_read(transceiver);

  ground_transceiver_handle_data(transceiver);

#ifdef _WIN32
  Sleep(10);
#else
  usleep(10000);
#endif
}

int ground_transceiver_handle_data(GroundTransceiver *transceiver) {
  // data[0] is code
  // data[0] == 1 => debug/error message -> just print it
  // data[0] == 2 => terminate
  // data[0] == ...
  uint8_t *data = transceiver->recvBuffer;

  if (data[0] == 0) {
    return 0;
  }

  if (data[0] == 1) {
    for (int i = 1; i < transceiver->bufferSize; i++) {
      printf("%c", data[i]);
    }
    printf("\n");
  }

  if (data[0] == 2) {
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
  int sentBytes = 0;
  while (sentBytes == 0) {
    sentBytes = writePort(transceiver->port, transceiver->sendBuffer,
                          transceiver->bufferSize);
  }
}

void ground_transceiver_read(GroundTransceiver *transceiver) {
  uint8_t *buffer = transceiver->recvBuffer;
  int readBytes = 0;
  int offset = 0;
  while (readBytes < 32) {
    readBytes +=
        readPort(transceiver->port, buffer + offset, transceiver->bufferSize);
    offset += readBytes;

    if (buffer[offset] == 0) {
      return;
    }

    if (readBytes < 0) {
      printf("Error reading from pico\n");
      return;
    }
  }
}
