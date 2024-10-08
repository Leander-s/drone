#include <controller.h>

Controller *controller_create(ControllerCreateInfo *createInfo) {
  Controller *result = (Controller *)malloc(sizeof(Controller));
  result->mode = Manual;

  // init motor id
  for (int i = 0; i < THROTTLE_MOTOR_N; i++) {
    result->throttleMotorIDs[i] = createInfo->throttleMotorIDs[i];
  }

  // init servo ids
  for (int i = 0; i < PITCH_SERVO_N; i++) {
    result->pitchServoIDs[i] = createInfo->pitchServoIDs[i];
  }
  for (int i = 0; i < YAW_SERVO_N; i++) {
    result->yawServoIDs[i] = createInfo->yawServoIDs[i];
  }
  for (int i = 0; i < ROLL_SERVO_N; i++) {
    result->rollServoIDs[i] = createInfo->rollServoIDs[i];
  }

  // init function
  result->servo_init = createInfo->servo_init;
  result->servo_turn = createInfo->servo_turn;
  result->motor_init = createInfo->motor_init;
  result->set_throttle = createInfo->set_throttle;

  // init control state
  result->controlState =
      (DroneControlState){.yaw = 0, .pitch = 0, .roll = 0, .throttle = 0};

  return result;
};

void controller_update(Controller *controller) {

  // need to do some math to interpolate between pitch and roll
  for (int i = 0; i < PITCH_SERVO_N; i++) {
    // turn pitch servos the way they have to turn
  }
  for (int i = 0; i < YAW_SERVO_N; i++) {
    // turn yaw servos the way they have to turn
  }
  for (int i = 0; i < ROLL_SERVO_N; i++) {
    // turn roll servos the way they have to turn
  }

  for (int i = 0; i < THROTTLE_MOTOR_N; i++) {
    controller->set_throttle(controller->throttleMotorIDs[i],
                             (float)controller->controlState.throttle / 255);
  }
}

void controller_destroy(Controller *controller) {
  for (int i = 0; i < THROTTLE_MOTOR_N; i++) {
    controller->set_throttle(controller->throttleMotorIDs[i], 0.0f);
  }
  free(controller);
};
