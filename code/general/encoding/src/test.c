#include "../include/encoding.h"

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
  uint8_t test[32];
  uint8_t testOther[32];
  memset(test, 0, 32);

  test[0] = 127;
  test[1] = 127;
  test[2] = 127;
  test[3] = 200;

  memcpy(testOther, test, 32);

  encode_buffer(test, 32);
  test_sendable("Sendable test", test, 32);

  decode_buffer(test, 32);
  test_equal("Equality test", test, testOther, 32);
  printf("This is the yawAngle: %f\n", (float)test[3]/255.0f * 180.0f);
}
