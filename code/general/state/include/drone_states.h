#pragma once
#include <stdint.h>
#include <general_math.h>

typedef struct {
  uint8_t pitch;
  uint8_t roll;
  uint8_t yaw;
  uint8_t throttle;
} DroneControlState;

typedef struct {
    Quaternion orientation;
    float altitude;
} DroneSensorState;
