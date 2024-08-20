#include "drone_protocol_config.h"

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

typedef struct {
  uint8_t pitch;
  uint8_t roll;
  uint8_t yaw;
  uint8_t throttle;
} DroneAction;

typedef struct {
  DroneAction *action;
  uint32_t bufferSize;
  char *sendBuffer;
  char *readBuffer;
  void (*send)(uint8_t *buffer, uint32_t len);
  void (*recv)(uint8_t *buffer, uint32_t len);
} DroneTransceiver;

typedef struct {
  void (*init)();
  void (*send)(uint8_t *buffer, uint32_t len);
  void (*recv)(uint8_t *buffer, uint32_t len);
  uint32_t bufferSize;
} DroneTransceiverCreateInfo;

DroneTransceiver *drone_protocol_init(DroneTransceiverCreateInfo *createInfo);
void drone_protocol_run(DroneTransceiver *transceiver);
void drone_protocol_handle_message(DroneTransceiver *transceiver);
void drone_protocol_terminate(DroneTransceiver *transceiver);

void drone_send(DroneTransceiver *transceiver);
void drone_read(DroneTransceiver *transceiver);
void drone_flush_tx(DroneTransceiver *transceiver);
void drone_flush_rx(DroneTransceiver *transceiver);
