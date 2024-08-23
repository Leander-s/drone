#include <debugging_util.h>
#include <hardware/timer.h>
#include <nrf24.h>
#include <pico/time.h>

#define READ_TIMEOUT_US 10000

typedef union {
  float f;
  uint8_t bytes[4];
} FloatBytes;

typedef union {
  int i;
  uint8_t bytes[4];
} IntBytes;

typedef struct {
  FloatBytes transmissionsPerSecond;
  IntBytes readTimeouts;
  IntBytes usbDisconnects;
} SystemLog;

void setup() {
  stdio_usb_init();

  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }

  while (!tud_cdc_connected()) {
    sleep_ms(100);
  }

  pico_debug_print("Pico connected", 32);

  nrf24_init();
}

void loop() {
  char fromPC[64];
  char toPC[64];
  memset(fromPC, 1, 64);
  memset(toPC, 0, 64);
  int result;
  SystemLog log = {
      .transmissionsPerSecond.f = 0, .readTimeouts.i = 0, .usbDisconnects.i = 0};
  while (1) {
    uint64_t start_time = time_us_64();
    result = pico_read(fromPC, 64);
    if (result < 0) {
      log.usbDisconnects.i++;
      int blink = 0;
      while (!stdio_usb_connected()) {
        gpio_put(LED_PIN, blink);
        blink = (blink + 1) % 2;
        sleep_ms(100);
      }
      while (!tud_cdc_connected()) {
        gpio_put(LED_PIN, blink);
        blink = (blink + 1) % 2;
        sleep_ms(100);
      }
      continue;
    }
    if (result < 32) {
      continue;
    }
    if ((fromPC[32] - 1) & 1) {
      log.usbDisconnects.i++;
      int blink = 0;
      while (!stdio_usb_connected()) {
        gpio_put(LED_PIN, blink);
        blink = (blink + 1) % 2;
        sleep_ms(100);
      }
      while (!tud_cdc_connected()) {
        gpio_put(LED_PIN, blink);
        blink = (blink + 1) % 2;
        sleep_ms(100);
      }
      pico_debug_print("Pico connected", 64);
      continue;
    }
    int bytesSent = nrf24_send((uint8_t *)fromPC, 32);
    int bytesReceived = nrf24_read((uint8_t *)toPC, 32, READ_TIMEOUT_US);
    if (bytesReceived == 0) {
      log.readTimeouts.i++;
    }
    uint64_t end_time = time_us_64();
    for(int i = 0; i < 4; i++){
        toPC[32 + i] = log.usbDisconnects.bytes[i];
    }

    for(int i = 0; i < 4; i++){
        toPC[36 + i] = log.readTimeouts.bytes[i];
    }

    log.transmissionsPerSecond.f = 1.0 / (float)(end_time - start_time);

    for(int i = 0; i < 4; i++){
        toPC[40 + i] = log.transmissionsPerSecond.bytes[i];
    }

    pico_print(toPC, 64);
  }
}

int main() {
  setup();
  loop();
  return 0;
}
