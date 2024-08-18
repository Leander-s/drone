#include "connection.h"
#include "input_poll.h"

int main() {
  int port = initConnection("/dev/ttyACM1");

  inputHandler *handler = createInputHandler();
  char pico_debug_message[128];
  char pico_send_buffer[128];

  while (1) {
    if (key_down(handler, XK_W)) {
      memcpy(pico_send_buffer, "test", 4);
      pico_write(port, pico_send_buffer);
      memset(pico_send_buffer, '\0', 128);
    }
    int read_bytes = pico_read(port, pico_debug_message, 128);
    if (read_bytes > 0) {
      if (strlen(pico_debug_message) < 1) {
        continue;
      }
      printf("%s\n", pico_debug_message);
      memset(pico_debug_message, '\0', 128);
    } else if (read_bytes < 0) {
      printf("Error while reading\n");
      memset(pico_debug_message, '\0', 128);
    }
  }

  return 0;
}
