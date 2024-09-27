#include <drone.h>

Drone *drone_start() {
  Drone *result = (Drone *)malloc(sizeof(Drone));
  result->state =
      (DroneControlState){.throttle = 0, .pitch = 0, .yaw = 0, .roll = 0};
  result->sensorState.altitude = 0;
  result->log.error = 0;
  result->sensor =
      (DroneSensor){.state = &result->sensorState, .log = &result->log};

  DroneTransceiverCreateInfo createInfo =
      (DroneTransceiverCreateInfo){
          .init = nrf24_init,
          .send = nrf24_send,
          .recv = nrf24_read,
          .bufferSize = 32,
          .log = &result->log,
          .sensorState = &result->sensorState,
          .controlState = &result->state,
      };
  result->transceiver = drone_protocol_init(&createInfo);
  sensor_init(&result->sensor);
  return result;
}

void drone_update(Drone *drone) {
  drone_protocol_update(drone->transceiver);
  sensor_update(&drone->sensor);

  // Now sensors should have updated the orientation of the drone, next up is
  // controls correcting towards target vector
}

void drone_end(Drone *drone) {
  drone_protocol_terminate(drone->transceiver);
  free(drone);
}
