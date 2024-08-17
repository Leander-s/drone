#include "connection.h"
#include "input_poll.h"

int main() {
  int port = initConnection("/dev/ttyACM0");

  inputHandler *handler = createInputHandler();

  while (1) {
    if (key_down(handler, XK_W)) {
      write(port, "test", 4);
    }
  }

  return 0;
}
