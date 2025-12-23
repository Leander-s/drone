#pragma once
#include <diagnostics.h>
#include <drone_states.h>
#include <general_math.h>
#include <hardware/i2c.h>
#include <hardware/structs/io_bank0.h>
#include <pico/stdlib.h>
#include <imu.h>

typedef struct{
    DroneSensorState *state;
    DroneSystemLog *log;
}DroneSensor;

void sensor_init(DroneSensor *sensor);

void sensor_update(DroneSensor *sensor);
