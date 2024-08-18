#include "connection.h"
#include "input_poll.h"

int main() {
  int port = initConnection("/dev/ttyACM0");

  inputHandler *handler = createInputHandler();
  char message[128];

  while (1) {
    int read_bytes = pico_read(port, message, 128);
    if (read_bytes > 0) {
      if (strlen(message) < 1) {
        continue;
      }
      printf("%s\n", message);
      memset(message, '\0', 128);
    } else if (read_bytes < 0) {
      printf("Error while reading\n");
      memset(message, '\0', 128);
    }
  }

  return 0;
}
