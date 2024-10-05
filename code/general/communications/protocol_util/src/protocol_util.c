#include "../include/protocol_util.h"

void encode_buffer(uint8_t *buffer, int len) {
  uint8_t *transBuffer = malloc(len);
  memset(transBuffer, 0, len);
  int bitIndex = 0;
  for (int i = 0; i < len; i++) {
    int bufferByte = bitIndex / 8;
    int firstBit = bitIndex % 8;
    int nextByteBits = firstBit - 1;
    transBuffer[i] |= (buffer[bufferByte] << firstBit) |
                      buffer[bufferByte + 1] >> (7 - nextByteBits) | 1;
    bitIndex += 7;
  }
  memcpy(buffer, transBuffer, len);
  free(transBuffer);
}

void decode_buffer(uint8_t *buffer, int len) {
  uint8_t *transBuffer = malloc(len);
  memset(transBuffer, 0, len);
  int bitIndex = 0;
  for (int i = 0; i < len - 4; i++) {
    int currentByte = bitIndex / 7;
    int firstBit = bitIndex % 7;
    int lastBit = firstBit;
    uint8_t Byte1Mask = 0xFE << firstBit;
    uint8_t Byte2Mask = (0xFF >> (7 - lastBit));
    transBuffer[i] = ((buffer[currentByte] << firstBit) & Byte1Mask) |
                     ((buffer[currentByte + 1] >> (7 - lastBit)) & Byte2Mask);

    bitIndex += 8;
  }
  memcpy(buffer, transBuffer, len);
  free(transBuffer);
}
