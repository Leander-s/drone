#pragma once
#include "imu.h"

typedef struct{
    DroneSensorState *state;
    DroneSystemLog *log;
}DroneSensor;

void sensor_init(DroneSensor *sensor);

void sensor_update(DroneSensor *sensor);
