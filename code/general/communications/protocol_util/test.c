#include "protocol_util.h"

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
  uint8_t testBuffer[32];
  memcpy(testBuffer + 1, "Hello", strlen("Hello"));
  testBuffer[0] = 1;

  uint8_t testCopy[32];
  memcpy(testCopy, testBuffer, 32);

  encode_buffer(testBuffer, 16);
  encode_buffer(testBuffer + 16, 16);
  test_sendable("Sendable test", testBuffer, 32);

  decode_buffer(testBuffer, 16);
  decode_buffer(testBuffer + 16, 16);
  test_equal("Equality test", testBuffer, testCopy, 32);
}
