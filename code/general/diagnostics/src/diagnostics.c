#include "diagnostics.h"

void read_float_bytes_f(FloatBytes *fb, const float f) { fb->f = f; }

void read_float_bytes_b(FloatBytes *fb, const uint8_t bytes[4]) {
  for (int i = 0; i < 4; i++) {
    fb->bytes[i] = bytes[i];
  }
}

void read_int_bytes_i(IntBytes *ib, const int i) { ib->i = i; }

void read_int_bytes_b(IntBytes *ib, const uint8_t bytes[4]) {
  for (int i = 0; i < 4; i++) {
    ib->bytes[i] = bytes[i];
  }
}
