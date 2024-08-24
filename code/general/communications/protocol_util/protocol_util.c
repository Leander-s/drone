#include "protocol_util.h"

void encode_buffer(uint8_t *buffer, int len) {
  IntBytes mask;
  mask.i = 0;
  for (int i = 0; i < len - 4; i++) {
    if (i % 8 == 0) {
      mask.i += 1 << i;
      continue;
    }
    if (buffer[i] == 0) {
      mask.i += 1 << i;
    }
  }
  for (int i = len - 4; i < len; i++) {
    buffer[i] = mask.bytes[i];
  }
}

void decode_buffer(uint8_t *buffer, int len) {
  IntBytes mask;
  for (int i = len - 4; i < len; i++) {
    mask.bytes[i] = buffer[i];
  }
  for (int i = 0; i < len - 4; i++) {
    if (i % 8 == 0) {
      continue;
    }
    if (mask.i & 1 << i) {
      // technically it should always become 0
      // but i am not brave enough to make use of this
      buffer[i] -= 1;
    }
  }
}
