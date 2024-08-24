#include "protocol_util.h"

void print_buffer(uint8_t *buffer, int len) {
  for (int i = 0; i < len; i++) {
    printf("Buffer[%u] = %u\n", i, buffer[i]);
  }
}

int buffers_are_equal(uint8_t *buffer1, uint8_t *buffer2, int len) {
  for (int i = 0; i < len; i++) {
    if (buffer1[i] != buffer2[i])
      return 0;
  }
  return 1;
}

int buffer_is_sendable(uint8_t *buffer, int len) {
  for (int i = 0; i < len; i++) {
    if (buffer[i] == 0)
      return 0;
  }
  return 1;
}

int main() {
  uint8_t testBuffer[32];
  testBuffer[1] = 1;
  testBuffer[8] = 128;
  testBuffer[9] = 255;
  testBuffer[10] = 1;
  testBuffer[18] = 100;
  testBuffer[25] = 13;

  uint8_t testCopy[32];
  memcpy(testCopy, testBuffer, 32);

  encode_buffer(testBuffer, 32);
  if (!buffer_is_sendable(testBuffer, 32)) {
    printf("Test 1 failed\n");
    print_buffer(testBuffer, 32);
  } else {
    printf("Test 1 passed\n");
  }

  decode_buffer(testBuffer, 32);
  if (!buffers_are_equal(testBuffer, testCopy, 32)) {
    printf("Test 2 failed\n");
    for (int i = 0; i < 32; i++) {
      printf("Buffer : %u, control : %u\n", testBuffer[i], testCopy[i]);
    }
  } else {
    printf("Test 2 passed\n");
  }
}
