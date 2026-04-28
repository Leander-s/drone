#pragma once
#include "controller.h"
#include "diagnostics.h"
#include "drone_states.h"
#include "nrf24.h"
#include "protocol.h"
#include "pwm_pico.h"
#include "sensors.h"

typedef struct {
  DroneTransceiver transceiver;
  DroneControlState state;
  DroneSystemLog log;
  DroneSensor sensor;
  DroneSensorState sensorState;
  Controller controller;
} Drone;

void drone_start(Drone *drone);

void drone_update(Drone *drone);

void drone_end(Drone *drone);
