#include <connection.h>

#ifdef _WIN32
HANDLE initConnection(const char* path){
    HANDLE hComm = CreateFile(path,                // Port name
        GENERIC_READ | GENERIC_WRITE, // Read/Write
        0,                            // No sharing
        NULL,                         // No security
        OPEN_EXISTING,                // Open existing port only
        0,                            // Non-overlapped I/O
        NULL);                        // Null for Comm devices

    if (hComm == INVALID_HANDLE_VALUE) {
        perror("Error opening COM port");
        return NULL;
    }

    // Set COM port parameters
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hComm, &dcbSerialParams)) {
        perror("Error getting COM port state");
        CloseHandle(hComm);
        return NULL;
    }

    dcbSerialParams.BaudRate = CBR_115200;  // Baud rate
    dcbSerialParams.ByteSize = 8;         // Byte size
    dcbSerialParams.StopBits = ONESTOPBIT;// Stop bits
    dcbSerialParams.Parity = NOPARITY;    // Parity

    if (!SetCommState(hComm, &dcbSerialParams)) {
        perror("Error setting COM port state");
        CloseHandle(hComm);
        return NULL;
    }

    // Set COM port timeouts
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hComm, &timeouts)) {
        perror("Error setting COM port timeouts");
        CloseHandle(hComm);
        return NULL;
    }
    return hComm;
}

int writePort(HANDLE port, uint8_t* buffer, unsigned long toWrite){
    unsigned long amount;
    if(!WriteFile(port, buffer, toWrite, &amount, NULL)){
        return -1;
    }
    return (int)amount;
}

int readPort(HANDLE port, uint8_t* buffer, unsigned long toRead){
    unsigned long amount;
    if(!ReadFile(port, buffer, toRead, &amount, NULL)){
        return -1;
    }
    return (int)amount;
}
#else
int initConnection(const char *path) {
  int port = open(path, O_RDWR);
  if (port < 0) {
    printf("Error while connecting: %s\n", strerror(errno));
    return 0;
  }
  printf("Connected successfully\n");

  struct termios tty;

  if (tcgetattr(port, &tty) != 0) {
    printf("Error from tcgetattr: %s\n", strerror(errno));
    return 0l;
  }

  tty.c_iflag &= ~PARENB;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CSIZE;
  tty.c_cflag |= CS8;
  tty.c_cflag &= ~CRTSCTS;
  tty.c_cflag |= CREAD | CLOCAL;
  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO;
  tty.c_lflag &= ~ECHOE;
  tty.c_lflag &= ~ECHONL;
  tty.c_lflag &= ~ISIG;
  tty.c_iflag &= ~(IXON | IXOFF | IXANY);
  tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
  tty.c_oflag &= ~OPOST;
  tty.c_oflag &= ~ONLCR;
  tty.c_cc[VTIME] = 1;
  tty.c_cc[VMIN] = 0;

  cfsetspeed(&tty, B115200);

  if (tcsetattr(port, TCSANOW, &tty) != 0) {
    printf("Error from tcsetattr: %s\n", strerror(errno));
  }

  return port;
}

int writePort(int port, uint8_t* buffer, int amount){
    return write(port, buffer, amount);
}

int readPort(int port, uint8_t* buffer, int amount){
    return read(port, (void*)buffer, amount);
}
#endif
