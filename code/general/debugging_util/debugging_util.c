#include "debugging_util.h"

int pico_read(char* readBuffer, int size){
    int result;
    int offset = 0;
    do{
    result = stdio_get_until(readBuffer + offset, size - offset, 1000);
    offset += result;
    if(result == PICO_ERROR_TIMEOUT) break;
    if(offset == 0) continue;
    if(offset >= size) break;
    if(readBuffer[offset - 1] == '\0') break;
    }while(1);
    return result;
}

void pico_print(char *message, int len){
    int printedBytes = 0;
    while(printedBytes == 0){
        printedBytes = printf("%.*s", len, message);
    }
}

void pico_debug_print(char *message, int len){
    char debug_message[128];
    memset(debug_message, 0, 128);
    memcpy(debug_message + 1, message, strlen(message));
    debug_message[0] = 1;
    printf("%.*s", len + 1, debug_message);
}
