#include <debugging_util.h>

int pico_read(uint8_t *readBuffer, int size) {
  int result;
  int i = 0;
  while (i < size) {
    readBuffer[i] = getchar();
    i++;
    if (result < 0)
      return -1;
  }
  return i;
}

int pico_print(uint8_t *message, int len) {
  int printedBytes = 0;
  for (int i = 0; i < len; i++) {
    printedBytes += printf("%hhu", message[i]);
  }
  return printedBytes;
}

void pico_debug_print(char *message, int len) {
  char debug_message[128];
  memset(debug_message, 0, 128);
  memcpy(debug_message + 1, message, strlen(message));
  debug_message[0] = 1;
  printf("%.*s", len + 1, debug_message);
}
