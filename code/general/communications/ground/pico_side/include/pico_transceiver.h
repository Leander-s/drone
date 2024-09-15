#include <debugging_util.h>
#include <diagnostics.h>
#include <hardware/timer.h>
#include <nrf24.h>
#include <pico/time.h>
#include <protocol_util.h>

#define READ_TIMEOUT_US 10000

typedef struct{
    uint8_t fromPC[64];
    uint8_t toPC[64];
    PicoSystemLog log;
}PicoTransceiver;

void busy_wait_for_connect();

PicoTransceiver *pico_transceiver_create();

void pico_transceiver_update(PicoTransceiver *trans);

void pico_transceiver_destroy(PicoTransceiver *trans);
