#include <controller.h>

Controller *controller_create() {
  Controller *result = (Controller *)malloc(sizeof(Controller));
  result->mode = Manual;
  result->throttleMotorID = 0;
  // servos now ...

  result->controlState =
      (DroneControlState){.yaw = 0, .pitch = 0, .roll = 0, .throttle = 0};

  return result;
};

void controller_update(Controller *controller) {

    // need to do some math to interpolate between pitch and roll
    for(int i = 0; i < PITCH_SERVO_N; i++){
        // turn pitch servos the way they have to turn
    }
    for(int i = 0; i < YAW_SERVO_N; i++){
        // turn yaw servos the way they have to turn
    }
    for(int i = 0; i < ROLL_SERVO_N; i++){
        // turn roll servos the way they have to turn
    }

    controller->set_throttle(controller->throttleMotorID, (float)controller->controlState.throttle/255);
}

void controller_destroy(Controller *controller){
    controller->set_throttle(controller->throttleMotorID, 0.0f);
    free(controller);
};
