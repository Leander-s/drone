#include <drone.h>

Drone *drone_start() {
  Drone *result = (Drone *)malloc(sizeof(Drone));
  result->sensorState.altitude = 0;
  result->log.error = 0;
  result->sensor =
      (DroneSensor){.state = &result->sensorState, .log = &result->log};

  // create drone controls
  ControllerCreateInfo controllerInfo =
      (ControllerCreateInfo){.current = &result->sensorState.orientation,
                             .motor_init = init_throttle,
                             .set_throttle = set_throttle,
                             .servo_init = init_servo,
                             .servo_turn = set_angle,
                             .throttleMotorIDs[0] = 0,
                             .leftPitchServoIDs[0] = 1,
                             .rightPitchServoIDs[0] = 2,
                             .yawServoIDs[0] = 3
      };
  result->controller = controller_create(&controllerInfo);
  result->state = &result->controller->controlState;

  // create drone transceiver
  DroneTransceiverCreateInfo createInfo = (DroneTransceiverCreateInfo){
      .init = nrf24_init,
      .send = nrf24_send,
      .recv = nrf24_read,
      .bufferSize = 32,
      .log = &result->log,
      .sensorState = &result->sensorState,
      .controlState = result->state,
  };
  result->transceiver = drone_protocol_init(&createInfo);
  sensor_init(&result->sensor);
  return result;
}

void drone_update(Drone *drone) {
  drone_protocol_update(drone->transceiver);
  sensor_update(&drone->sensor);
  controller_update(drone->controller);
}

void drone_end(Drone *drone) {
  drone_protocol_terminate(drone->transceiver);
  controller_destroy(drone->controller);
  free(drone);
}
