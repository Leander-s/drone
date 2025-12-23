#include "connection.h"

int main(){
#ifdef _WIN32
    HANDLE port;
    const char* path = "COM4";
#else
    int port;
    const char* path = "/dev/ttyACM1";
#endif

    port = initConnection(path);

    char message[32];
    
    while(1){
        int readBytes = readPort(port, (uint8_t*)message, 32);
        if(readBytes > 0){
            printf("%.*s\n", 32, (char*)message);
            memset(message, 0, 32);
        }
    }
}
