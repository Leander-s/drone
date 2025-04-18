#pragma once
#include <connection.h>

typedef struct {
#ifdef _WIN32
  HANDLE port;
#else
  uint32_t port;
#endif
  uint32_t bufferSize;
  DroneControlState *controlState;
  DroneSensorState *sensorState;
  uint8_t *sendBuffer;
  uint8_t *recvBuffer;
  PCSystemLog log;
} GroundTransceiver;

typedef struct {
  const char *path_to_port;
  uint32_t bufferSize;
  DroneControlState* controlState;
  DroneSensorState* sensorState;
} GroundTransceiverCreateInfo;

GroundTransceiver *ground_transceiver_create(GroundTransceiverCreateInfo *info);
void ground_transceiver_run(GroundTransceiver *transceiver);
int ground_transceiver_update(GroundTransceiver *transceiver);
void ground_transceiver_destroy(GroundTransceiver *transceiver);

int ground_transceiver_send(GroundTransceiver *transceiver);
int ground_transceiver_read(GroundTransceiver *transceiver);
int ground_transceiver_handle_data(GroundTransceiver *transceiver);
