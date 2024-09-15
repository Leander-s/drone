#include <controls_math.h>

// Controls
float approximate(float current, float target, float cutoff) {
    float diff = target - current;
    if(diff == 0) return 0;

    float absDiff = float_abs(diff);
    float sign = diff / absDiff;
    /*
    // Using linear function
    return sign * min(absDiff/cutoff, 1.0f);
    */

    /*
    // Using x² function to get a falloff
    float factor = 1 / (pow(cutoff, 2));
    return sign * min(factor * (pow(absDiff, 2)), 1.0f);
    */

    // Using 2^x function to get a falloff
    float factor = 1/(pow(2, cutoff) - 1);
    return sign * min(factor * (pow(2, absDiff) - 1), 1.0f);
}

// Util
float max(float a, float b) {
    if (a > b)
        return a;
    return b;
}

float min(float a, float b) {
    if (a < b)
        return a;
    return b;
}

float float_abs(float x) { return x < 0 ? -x : x; }
