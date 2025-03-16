#include <connection.h>

#ifdef _WIN32
const char *find_device_path(const char *name)
{
  HANDLE hComm;
  char portName[15];

  for (int i = 1; i <= 256; i++)
  {
    snprintf(portName, sizeof(portName), "\\\\.\\COM%d", i);
    hComm = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    char handshake[10];
    char response[33];
    memset(response, 0, 33);
    memset(handshake, 4, 10);
    handshake[9] = 0;
    writePort(hComm, (uint8_t *)handshake, 10);

    // read response
    int offset = 0;
    while (offset < 33)
    {
      int read = readPort(hComm, response + offset, 33);
      if (read < 0)
      {
        printf("Error reading from %s\n", portName);
        break;
      }
      if (read == 0)
      {
        break;
      }
      offset += read;
    }
    if (offset == 0)
    {
      continue;
    }

    // check response
    if (strcmp(response + 1, "Pico connected") == 0)
    {
      printf("Found device: %s -> %s\n", name, portName);
      CloseHandle(hComm);
      break;
    }

    CloseHandle(hComm);
  }
  return portName;
}

HANDLE initConnection(const char *path)
{
  HANDLE hComm = CreateFile(path,                         // Port name
                            GENERIC_READ | GENERIC_WRITE, // Read/Write
                            0,                            // No sharing
                            NULL,                         // No security
                            OPEN_EXISTING,                // Open existing port only
                            0,                            // Non-overlapped I/O
                            NULL);                        // Null for Comm devices

  if (hComm == INVALID_HANDLE_VALUE)
  {
    perror("Error opening COM port");
    return NULL;
  }

  // Set COM port parameters
  DCB dcbSerialParams = {0};
  dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

  if (!GetCommState(hComm, &dcbSerialParams))
  {
    perror("Error getting COM port state");
    CloseHandle(hComm);
    return NULL;
  }

  dcbSerialParams.BaudRate = CBR_115200; // Baud rate
  dcbSerialParams.ByteSize = 8;          // Byte size
  dcbSerialParams.StopBits = ONESTOPBIT; // Stop bits
  dcbSerialParams.Parity = NOPARITY;     // Parity

  if (!SetCommState(hComm, &dcbSerialParams))
  {
    perror("Error setting COM port state");
    CloseHandle(hComm);
    return NULL;
  }

  // Set COM port timeouts
  COMMTIMEOUTS timeouts = {0};
  timeouts.ReadIntervalTimeout = 50;
  timeouts.ReadTotalTimeoutConstant = 50;
  timeouts.ReadTotalTimeoutMultiplier = 10;

  if (!SetCommTimeouts(hComm, &timeouts))
  {
    perror("Error setting COM port timeouts");
    CloseHandle(hComm);
    return NULL;
  }
  return hComm;
}

int writePort(HANDLE port, uint8_t *buffer, unsigned long toWrite)
{
  unsigned long amount;
  if (!WriteFile(port, buffer, toWrite, &amount, NULL))
  {
    return -1;
  }
  return (int)amount;
}

int readPort(HANDLE port, uint8_t *buffer, unsigned long toRead)
{
  unsigned long amount;
  if (!ReadFile(port, buffer, toRead, &amount, NULL))
  {
    return -1;
  }
  return (int)amount;
}
#else
const char *find_device_path(const char *name)
{
  struct udev *udev;
  struct udev_enumerate *enumerate;
  struct udev_list_entry *devices, *dev_list_entry;
  struct udev_device *dev;
  const char *value = NULL;

  udev = udev_new();
  if (!udev)
  {
    fprintf(stderr, "Cannot create udev object\n");
    udev_unref(udev);
    return NULL;
  }

  enumerate = udev_enumerate_new(udev);
  udev_enumerate_add_match_subsystem(enumerate, "tty");
  udev_enumerate_scan_devices(enumerate);

  devices = udev_enumerate_get_list_entry(enumerate);

  udev_list_entry_foreach(dev_list_entry, devices)
  {
    const char *path;
    path = udev_list_entry_get_name(dev_list_entry);
    dev = udev_device_new_from_syspath(udev, path);
    struct udev_device *parent_dev =
        udev_device_get_parent_with_subsystem_devtype(dev, "usb", "usb_device");

    if (parent_dev)
    {
      const char *device = udev_device_get_sysattr_value(parent_dev, "product");
      if (device && strcmp(device, name) == 0)
      {
        value = udev_device_get_devnode(dev);
        if (value)
        {
          int port = initConnection(value);

          // Do a handshake
          char handshake[10];
          char response[33];
          memset(response, 0, 33);
          memset(handshake, 4, 10);
          handshake[9] = 0;
          writePort(port, (uint8_t *)handshake, 10);

          // read response
          int offset = 0;
          while (offset < 33)
          {
            int read = readPort(port, response + offset, 33);
            if (read < 0)
            {
              printf("Error reading from %s\n", value);
              break;
            }
            if (read == 0)
            {
              break;
            }
            offset += read;
          }
          if (offset == 0)
          {
            continue;
          }

          // check response
          if (strcmp(response + 1, "Pico connected") == 0)
          {
            printf("Found device: %s -> %s\n", name, value);
            close(port);
            break;
          }

          // close wrong port
          close(port);
        }
      }
    }
    udev_device_unref(dev);
  }

  udev_enumerate_unref(enumerate);
  udev_unref(udev);

  return value;
}

int initConnection(const char *path)
{
  int port = open(path, O_RDWR);
  if (port < 0)
  {
    printf("Error while connecting to %s : %s\n", path, strerror(errno));
    return 0;
  }
  printf("Connected successfully\n");

  struct termios tty;

  if (tcgetattr(port, &tty) != 0)
  {
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

  if (tcsetattr(port, TCSANOW, &tty) != 0)
  {
    printf("Error from tcsetattr: %s\n", strerror(errno));
  }

  return port;
}

int writePort(int port, uint8_t *buffer, int amount)
{
  return write(port, (void *)buffer, amount);
}

int readPort(int port, uint8_t *buffer, int amount)
{
  return read(port, (void *)buffer, amount);
}
#endif
