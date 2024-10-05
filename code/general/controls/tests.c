#include "include/controls_math.h"
#include <stdio.h>

void float_test(const char* testName, float result, float expected){
    if(result == expected){
        printf("%s passed!\n", testName);
        return;
    }
    printf("%s failed. Expected: %f, but got %f!\n", testName, expected, result);
}

int main(){
    float absTest1 = float_abs(-3);
    float_test("Abs test 1", absTest1, 3.0f);

    float maxTest1 = max(2, 1);
    float_test("Max test 1", maxTest1, 2.0f);

    float minTest1 = min(2, 1);
    float_test("Min test 1", minTest1, 1.0f);

    // actual funcionality
    float current = 2.5;
    float target = 10;
    float addRate = 3.0f;

    for(int i = 0; i < 100; i++){
        current += addRate * approximate(current, target, addRate);
        printf("Current state: Current (%f), target (%f)\n", current, target);
        if(i%20==0) target += 20;
    }
}
