#include "debugging_util.h"

int pico_read(char* readBuffer, int size){
    int result;
    int i = 0;
    while (i < size){
    readBuffer[i] = getchar();
    i++;
    if(result < 0) return -1;
    }
    return i;
}

int pico_print(char *message, int len){
    int printedBytes = printf("%.*s", len, message);
    if(printedBytes < 0){
        printf("Error while printing");
        gpio_put(25,1);
    }
    return printedBytes;
}

void pico_debug_print(char *message, int len){
    char debug_message[128];
    memset(debug_message, 0, 128);
    memcpy(debug_message + 1, message, strlen(message));
    debug_message[0] = 1;
    printf("%.*s", len + 1, debug_message);
}
