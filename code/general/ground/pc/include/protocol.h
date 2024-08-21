#include "connection.h"
#include "input_poll.h"

typedef struct {
    uint8_t pitch;
    uint8_t roll;
    uint8_t yaw;
    uint8_t throttle;
} DroneState;

typedef struct {
  uint32_t port;
  uint32_t bufferSize;
  DroneState currentState;
  uint8_t *sendBuffer;
  uint8_t *recvBuffer;
} GroundTransceiver;

typedef struct {
  char* path_to_port;
  uint32_t bufferSize;
} GroundTransceiverCreateInfo;

GroundTransceiver *ground_transceiver_create(GroundTransceiverCreateInfo *info);
void ground_transceiver_run(GroundTransceiver *transceiver);
void ground_transceiver_destroy(GroundTransceiver *transceiver);

void ground_transceiver_send(GroundTransceiver *transceiver);
void ground_transceiver_read(GroundTransceiver *transceiver);
int ground_transceiver_handle_data(GroundTransceiver *transceiver);
