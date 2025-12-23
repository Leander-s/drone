#pragma once
#include <diagnostics.h>
#include <drone_states.h>
#include <nrf24.h>
#include <protocol.h>
#include <sensors.h>
#include <controller.h>
#include <pwm_pico.h>

typedef struct {
    DroneTransceiver *transceiver;
    DroneControlState *state;
    DroneSystemLog log;
    DroneSensor sensor;
    DroneSensorState sensorState;
    Controller *controller;
} Drone;

Drone *drone_start();

void drone_update(Drone *drone);

void drone_end(Drone *drone);
