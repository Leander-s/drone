#include <controller.h>

Controller *controller_create(ControllerCreateInfo *createInfo) {
  Controller *result = (Controller *)malloc(sizeof(Controller));
  result->mode = Manual;

  // init function
  result->servo_init = createInfo->servo_init;
  result->servo_turn = createInfo->servo_turn;
  result->motor_init = createInfo->motor_init;
  result->set_throttle = createInfo->set_throttle;

  // init motor id and run function
  for (int i = 0; i < THROTTLE_MOTOR_N; i++) {
    result->throttleMotorIDs[i] = createInfo->throttleMotorIDs[i];
    result->motor_init(result->throttleMotorIDs[i]);
  }

  // init servo ids
  for (int i = 0; i < PITCH_SERVO_LEFT_N; i++) {
    result->leftPitchServoIDs[i] = createInfo->leftPitchServoIDs[i];
    result->servo_init(result->leftPitchServoIDs[i]);
  }
  for (int i = 0; i < PITCH_SERVO_RIGHT_N; i++) {
    result->rightPitchServoIDs[i] = createInfo->rightPitchServoIDs[i];
    result->servo_init(result->rightPitchServoIDs[i]);
  }
  for (int i = 0; i < YAW_SERVO_N; i++) {
    result->yawServoIDs[i] = createInfo->yawServoIDs[i];
    result->servo_init(result->yawServoIDs[i]);
  }

  // init control state
  result->controlState =
      (DroneControlState){.yaw = 127, .pitch = 127, .roll = 127, .throttle = 0};

  return result;
};

void controller_update(Controller *controller) {

  // map controls to angle
  float yawAngle = (float)(controller->controlState.yaw) / 255 * 180;
  float pitchAngle = (float)(controller->controlState.pitch) / 255 * 180;
  float rollAngle = (float)(controller->controlState.roll - 127) / 255 * 180;

  // need to do some math to interpolate between pitch and roll
  for (int i = 0; i < PITCH_SERVO_LEFT_N; i++) {
    // turn pitch servos the way they have to turn
    float angle = clamp_float(pitchAngle + rollAngle, 0.0f, 180.0f);
    controller->servo_turn(controller->leftPitchServoIDs[i], angle);
  }
  for (int i = 0; i < PITCH_SERVO_RIGHT_N; i++) {
    // turn pitch servos the way they have to turn
    float angle = clamp_float(pitchAngle - rollAngle, 0.0f, 180.0f);
    controller->servo_turn(controller->rightPitchServoIDs[i], angle);
  }
  for (int i = 0; i < YAW_SERVO_N; i++) {
    // turn yaw servos the way they have to turn
    controller->servo_turn(controller->yawServoIDs[i], yawAngle);
  }

  for (int i = 0; i < THROTTLE_MOTOR_N; i++) {
    controller->set_throttle(controller->throttleMotorIDs[i],
                             (float)(controller->controlState.throttle) / 255);
  }
}

void controller_destroy(Controller *controller) {
  for (int i = 0; i < THROTTLE_MOTOR_N; i++) {
    controller->set_throttle(controller->throttleMotorIDs[i], 0.0f);
  }
  free(controller);
};
