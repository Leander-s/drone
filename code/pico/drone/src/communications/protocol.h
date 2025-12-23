#pragma once

#ifdef NDEBUG
#define LOG(x, y)
#else
#define LOG(x, y) pico_print(x, y)
#endif

#include <class/cdc/cdc_device.h>
#include <debugging_util.h>
#include <diagnostics.h>
#include <drone_states.h>
#include <pico/stdio_usb.h>
#include <pico/time.h>
#include <encoding.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RADIO_READ_TIMEOUT 1000

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
        DroneSensorState *sensorState;
        DroneSystemLog *systemLog;
        DroneControlState *controlState;
        uint32_t bufferSize;
        uint8_t *sendBuffer;
        uint8_t *readBuffer;
        int (*send)(uint8_t *buffer, uint32_t len);
        int (*recv)(uint8_t *buffer, uint32_t len, int timeout_us);
} DroneTransceiver;

typedef struct {
        void (*init)();
        int (*send)(uint8_t *buffer, uint32_t len);
        int (*recv)(uint8_t *buffer, uint32_t len, int timeout_us);
        uint32_t bufferSize;
        DroneSensorState *sensorState;
        DroneSystemLog *log;
        DroneControlState *controlState;
} DroneTransceiverCreateInfo;

DroneTransceiver *drone_protocol_init(DroneTransceiverCreateInfo *createInfo);
void drone_protocol_run(DroneTransceiver *transceiver);
void drone_protocol_update(DroneTransceiver *transceiver);
void drone_protocol_handle_message(DroneTransceiver *transceiver);
void drone_protocol_prepare_data(DroneTransceiver *transceiver);
void drone_protocol_terminate(DroneTransceiver *transceiver);

int drone_send(DroneTransceiver *transceiver);
int drone_read(DroneTransceiver *transceiver);
void drone_flush_tx(DroneTransceiver *transceiver);
void drone_flush_rx(DroneTransceiver *transceiver);
