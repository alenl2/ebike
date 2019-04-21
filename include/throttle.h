#ifndef THROTTLE_H
#define THROTTLE_H
#include <Arduino.h>

void throttle_init();
void throttle_update();

struct ThrottleData {
    bool throttleOn; //display uart tx buffer
    int throttleValue;
    int minValue;
    int maxValue;
    float procentual;
};

extern ThrottleData throttleData;

#endif // KEY_H