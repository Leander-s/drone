#pragma once
#include <config.h>

typedef struct {
    DroneTransceiver *transceiver;
    DroneSensorState sensorState;
    DroneControlState state;
    DroneSystemLog log;
    Sensor sensor;
} Drone;

Drone *drone_start();

void drone_update(Drone *drone);

void drone_end(Drone *drone);
