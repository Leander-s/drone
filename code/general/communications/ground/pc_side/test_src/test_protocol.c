#include <input_poll.h>
#include <protocol.h>

GroundTransceiver *
ground_transceiver_create(GroundTransceiverCreateInfo *info) {
  GroundTransceiver *result = malloc(sizeof(GroundTransceiver));
  result->log = (PCSystemLog){
      .transmissionsPerSecond = 0,
      .usbDisconnects = 0,
      .picoReadTimeouts = 0,
      .usbReadErrors = 0,
      .usbWriteErrors = 0,
  };
  result->sendBuffer = malloc(info->bufferSize);
  result->recvBuffer = malloc(info->bufferSize);
  result->port = initConnection(info->path_to_port);
  result->bufferSize = info->bufferSize;
  result->controlState = info->controlState;
  result->sensorState = info->sensorState;

  ground_transceiver_read(result);
  printf("%s\n", result->recvBuffer);

  return result;
}

// this is for unit testing and multithreading
// Multithreading would still require a way to synchronize data since the
// structs are shared memory
void ground_transceiver_run(GroundTransceiver *transceiver) {
  printf("Ground transceiver running\n");
  // input handler initialized here because it should not be in this file
  // forever
  // The ground_transceiver_protocol should only serialize the data from the
  // DroneControlState, send it to the drone, and the deserialize the reply
  // into the DroneSensorState
  inputHandler *input = createInputHandler();

  while (1) {
    DroneControlState* controlState = transceiver->controlState;
    DroneSensorState* sensorState = transceiver->sensorState;
    uint32_t bufferSize = transceiver->bufferSize;
    uint8_t *sendBuffer = transceiver->sendBuffer;
    memset(sendBuffer, 0, transceiver->bufferSize);

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
    sendBuffer[0] = controlState->throttle;
    sendBuffer[1] = controlState->pitch;
    sendBuffer[2] = controlState->roll;
    sendBuffer[3] = controlState->yaw;

    int result;
    result = ground_transceiver_send(transceiver);
    if (result < 0) {
      transceiver->log.usbWriteErrors++;
    }

    uint8_t *recvBuffer = transceiver->recvBuffer;
    memset(recvBuffer, 0, transceiver->bufferSize);

    result = ground_transceiver_read(transceiver);
    if (result < 0) {
      transceiver->log.usbReadErrors++;
    }

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
    printf("No data\n");
    return 0;
  }

  if (data[0] == 1) {
    for (int i = 1; i < transceiver->bufferSize; i++) {
      printf("%c", data[i]);
    }
    printf("\n");
  }

  if (data[0] == 2) {
    printf("Leaving\n");
    return 1;
  }

  if (data[0] == 3) {
    // update sensor state
    // ...
  }

  // update log
  PicoSystemLog picoLog;
  read_int_bytes_b(&picoLog.usbDisconnects, &data[32]);
  read_int_bytes_b(&picoLog.readTimeouts, &data[36]);
  read_float_bytes_b(&picoLog.transmissionsPerSecond, &data[40]);

  transceiver->log.usbDisconnects = picoLog.usbDisconnects.i;
  transceiver->log.picoReadTimeouts = picoLog.readTimeouts.i;
  transceiver->log.transmissionsPerSecond = picoLog.transmissionsPerSecond.f;
  printf("%f\n", transceiver->log.transmissionsPerSecond);

  return 0;
}

void ground_transceiver_destroy(GroundTransceiver *transceiver) {
  free(transceiver->sendBuffer);
  free(transceiver->recvBuffer);
  free(transceiver);
}

int ground_transceiver_send(GroundTransceiver *transceiver) {
  int sentBytes = 0;
  encode_buffer(transceiver->sendBuffer, transceiver->bufferSize);
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
  while (offset < 32) {
    readBytes =
        readPort(transceiver->port, buffer + offset, transceiver->bufferSize);
    offset += readBytes;

    if (readBytes == 0) {
      return 0;
    }

    if (readBytes < 0) {
      printf("Error reading from pico\n");
      return -1;
    }
  }
  decode_buffer(transceiver->recvBuffer, transceiver->bufferSize);
  return offset;
}
