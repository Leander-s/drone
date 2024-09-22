#include "general_math.h"
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
  DroneControlState initialState =
      (DroneControlState){.throttle = 0, .pitch = 127, .roll = 127, .yaw = 127};
  *result->controlState = initialState;
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

  while (1) {
    if (ground_transceiver_update(transceiver)) {
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
int ground_transceiver_update(GroundTransceiver *transceiver) {
  DroneControlState *controlState = transceiver->controlState;
  DroneSensorState *sensorState = transceiver->sensorState;
  uint32_t bufferSize = transceiver->bufferSize;
  uint8_t *sendBuffer = transceiver->sendBuffer;
  uint8_t *recvBuffer = transceiver->recvBuffer;
  memset(sendBuffer, 0, transceiver->bufferSize);
  memset(recvBuffer, 0, transceiver->bufferSize);

  // sending/receiving data
  sendBuffer[0] = controlState->throttle;
  sendBuffer[1] = controlState->pitch;
  sendBuffer[2] = controlState->roll;
  sendBuffer[3] = controlState->yaw;

  int result;
  result = ground_transceiver_send(transceiver);
  if (result < 0) {
    transceiver->log.usbWriteErrors++;
  }

  result = ground_transceiver_read(transceiver);
  if (result < 0) {
    transceiver->log.usbReadErrors++;
  }

  return ground_transceiver_handle_data(transceiver);
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

  // printing for testing
  /*
  for(int i = 0; i < 64; i++){
      printf("(%d)[%hhu] ", i, data[i]);
  }
  printf("\n---------------------------\n");
  */

  // update log
  PicoSystemLog picoLog;
  read_int_bytes_b(&picoLog.usbDisconnects, data + 32);
  read_int_bytes_b(&picoLog.readTimeouts, data + 36);
  read_float_bytes_b(&picoLog.transmissionsPerSecond, data + 40);

  transceiver->log.usbDisconnects = picoLog.usbDisconnects.i;
  transceiver->log.picoReadTimeouts = picoLog.readTimeouts.i;
  transceiver->log.transmissionsPerSecond = picoLog.transmissionsPerSecond.f;

  if (data[0] == 0) {
    printf("No data\n");
    return 0;
  }

  if (data[0] == 1) {
    printf("%s\n", data + 1);
  }

  if (data[0] == 2) {
    printf("Terminating\n");
    return 1;
  }

  if (data[0] == 3) {
    // update sensor state
    memcpy(transceiver->sensorState->bytes, data + 1, 8);
    transceiver->sensorState->orientation.x =
        (float)((int16_t)((data[2] << 8) | data[1])) / 16384.0f;
    transceiver->sensorState->orientation.i =
        (float)((int16_t)((data[4] << 8) | data[3])) / 16384.0f;
    transceiver->sensorState->orientation.j =
        (float)((int16_t)((data[6] << 8) | data[5])) / 16384.0f;
    transceiver->sensorState->orientation.k =
        (float)((int16_t)((data[8] << 8) | data[7])) / 16384.0f;
  }

  return 0;
}

void ground_transceiver_destroy(GroundTransceiver *transceiver) {
  memset(transceiver->sendBuffer, 0, transceiver->bufferSize);
  transceiver->sendBuffer[36] = 1;
  ground_transceiver_send(transceiver);
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
  while (offset < transceiver->bufferSize) {
    readBytes =
        readPort(transceiver->port, buffer + offset, transceiver->bufferSize);
    offset += readBytes;

    if (readBytes == 0) {
      break;
    }

    if (readBytes < 0) {
      printf("Error reading from pico\n");
      return -1;
    }
  }
  // decode drone data
  decode_buffer(buffer, 32);
  // decode rest of buffer (just system logs right now)
  decode_buffer(buffer + 32, transceiver->bufferSize - 32);
  return offset;
}
