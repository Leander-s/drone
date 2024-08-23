#include "protocol.h"

GroundTransceiver *
ground_transceiver_create(GroundTransceiverCreateInfo *info) {
  GroundTransceiver *result = malloc(sizeof(GroundTransceiver));
  result->log = (PCSystemLog){
      .transmissionsPerSecond = 0, .usbDisconnects = 0, .picoReadTimeouts = 0,};
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
// Multithreading would still require a way to synchronize data since the
// structs are shared memory
void ground_transceiver_run(GroundTransceiver *transceiver) {
  printf("Ground transceiver running\n");

  while (1) {
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
  // data[0] == 2 => terminate (only important for multithreading/run-function
  // data[0] == 3 => normal sensor data
  //
  // data[1] - data[31] is sensor data
  //
  // data[32] - data[63] is log data
  // log data is always sent, no matter what code
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

  if (data[0] == 3) {
    // update sensor state
    // ...

    // update log
    PicoSystemLog picoLog;
    read_int_bytes_b(&picoLog.usbDisconnects, &data[32]);
    read_int_bytes_b(&picoLog.readTimeouts, &data[36]);
    read_float_bytes_b(&picoLog.transmissionsPerSecond, &data[40]);

    transceiver->log.usbDisconnects = picoLog.usbDisconnects.i;
    transceiver->log.picoReadTimeouts = picoLog.readTimeouts.i;
    transceiver->log.transmissionsPerSecond = picoLog.transmissionsPerSecond.f;
  }

  return 0;
}

void ground_transceiver_destroy(GroundTransceiver *transceiver) {
  free(transceiver->sendBuffer);
  free(transceiver->recvBuffer);
  free(transceiver);
}

int ground_transceiver_send(GroundTransceiver *transceiver) {
  int sentBytes = 0;
  while (sentBytes == 0) {
    sentBytes = writePort(transceiver->port, transceiver->sendBuffer,
                          transceiver->bufferSize);
  }
  return sentBytes;
}

int ground_transceiver_read(GroundTransceiver *transceiver) {
  uint8_t *buffer = transceiver->recvBuffer;
  int readBytes = 0;
  int offset = 0;
  while (readBytes < 32) {
    readBytes +=
        readPort(transceiver->port, buffer + offset, transceiver->bufferSize);
    offset += readBytes;

    if (buffer[offset] == 0) {
      return 0;
    }

    if (readBytes < 0) {
      printf("Error reading from pico\n");
      return -1;
    }
  }
  return readBytes;
}
