#include "../include/protocol_util.h"

typedef union {
  float f;
  uint8_t bytes[4];
} FloatBytes;

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

void test_sendable(const char *testName, uint8_t *buffer, int len) {
  if (buffer_is_sendable(buffer, len)) {
    printf("%s passed.\n", testName);
    return;
  }
  printf("%s failed. Buffer:\n", testName);
  print_buffer(buffer, len);
}

void test_equal(const char *testName, uint8_t *buffer1, uint8_t *buffer2,
                int len) {
  if (buffers_are_equal(buffer1, buffer2, len)) {
    printf("%s passed.\n", testName);
    return;
  }
  printf("%s failed. Buffers:\n", testName);
  for (int i = 0; i < len; i++) {
    printf("Buffer1[%d] : %u | Buffer2[%d] : %u\n", i, buffer1[i], i, buffer2[i]);
  }
}

int main() {
  uint8_t test[64];
  memset(test, 1, 64);
  test[0] = 3;
  memset(test + 28, 0, 4);
  memset(test + 60, 0, 4);
  test[4] = 255;
  test[50] = 255;

  uint8_t testCopy[64];
  memcpy(testCopy, test, 64);

  encode_buffer(test, 32);
  encode_buffer(test + 32, 32);
  test_sendable("Sendable test", test, 64);

  decode_buffer(test, 32);
  decode_buffer(test + 32, 32);
  test_equal("Equality test", test, testCopy, 64);
  printf("This is the first : %d\n", test[0]);
}
