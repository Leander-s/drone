#include "controller_config.h"
#include "controls_math.h"

typedef struct {
    Quaternion target;
    Quaternion current;
    /*
     * For multithreading
    Quaternion *target;
    Quaternion *current;
    */
    Quaternion targetRate;
    Quaternion currentRate;

    // Not sure how many servos for what
    // Not sure how the wings will look
    int pitchServoIDs[2];
    int yawServoIDs;
    int rollServoIDs[2];

    // function pointer to the function to turn a servo by an amount/with
    // a turn speed
    void (*servo_turn)(int id, int turn);
} Controller;

Controller* controller_create();

// In case we use multithreading
void controller_run();

void controller_update(Quaternion *current, Quaternion *target);

void controller_destroy(Controller* controller);
