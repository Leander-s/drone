For controls:
    Get in data from kalman filter/sensors:
    move flight surfaces to correct for errors
    get data from transceiver to change target vector
    Essentially we have a target, and get measurements to know our current
    we constantly correct current towards target
    when we get control input, we change our target, so the drone automatically
    corrects its orientation to match the target.

Need a struct to store target info
need a struct to store current info
need a function to update target info
need a function to update current info
correct for target in main function or make function to correct
