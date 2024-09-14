#include <diagnostics.h>

void read_float_bytes_f(FloatBytes* fb, float f){
    fb->f = f;
}

void read_float_bytes_b(FloatBytes* fb, uint8_t bytes[4]){
    for(int i = 0; i < 4; i++){
        fb->bytes[i] = bytes[i];
    }
}

void read_int_bytes_i(IntBytes* ib, int i){
    ib->i = i;
}

void read_int_bytes_b(IntBytes* ib, uint8_t bytes[4]){
    for(int i = 0; i < 4; i++){
        ib->bytes[i] = bytes[i];
    }
}
