#pragma once
#include "imu.h"

typedef struct{
    DroneSensorState *state;
    DroneSystemLog *log;
}Sensor;

void sensor_init(Sensor *sensor);

void sensor_update(Sensor *sensor);
