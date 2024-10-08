#pragma once
#include <config.h>

typedef struct {
    DroneTransceiver *transceiver;
    DroneSensorState sensorState;
    DroneControlState *state;
    DroneSystemLog log;
    DroneSensor sensor;
    Controller *controller;
} Drone;

Drone *drone_start();

void drone_update(Drone *drone);

void drone_end(Drone *drone);
