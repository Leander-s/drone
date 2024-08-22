#include "connection.h"
#include "input_poll.h"

typedef struct {
  uint8_t pitch;
  uint8_t roll;
  uint8_t yaw;
  uint8_t throttle;
} DroneControlState;

typedef struct {
} DroneSensorState;

typedef struct {
#ifdef _WIN32
  HANDLE port;
#else
  uint32_t port;
#endif
  uint32_t bufferSize;
  DroneControlState* controlState;
  DroneSensorState* sensorState;
  uint8_t *sendBuffer;
  uint8_t *recvBuffer;
} GroundTransceiver;

typedef struct {
  char *path_to_port;
  uint32_t bufferSize;
  DroneControlState* controlState;
  DroneSensorState* sensorState;
} GroundTransceiverCreateInfo;

GroundTransceiver *ground_transceiver_create(GroundTransceiverCreateInfo *info);
void ground_transceiver_run(GroundTransceiver *transceiver);
void ground_transceiver_update(GroundTransceiver *transceiver);
void ground_transceiver_destroy(GroundTransceiver *transceiver);

void ground_transceiver_send(GroundTransceiver *transceiver);
void ground_transceiver_read(GroundTransceiver *transceiver);
int ground_transceiver_handle_data(GroundTransceiver *transceiver);
