#include <hardware/timer.h>
#include <pico/time.h>

#include "debugging_util.h"
#include "diagnostics.h"
#include "nrf24.h"

#define READ_TIMEOUT 100000

typedef struct {
  uint8_t fromPC[64];
  uint8_t toPC[64];
  PicoSystemLog log;
} PicoTransceiver;

void busy_wait_for_connect();

void pico_transceiver_init(PicoTransceiver *trans);

void pico_transceiver_update(PicoTransceiver *trans);
