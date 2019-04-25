#ifndef BRAKE_H
#define BRAKE_H
#include <Arduino.h>

void brake_init();
void brake_update();

struct BrakeData {
    bool brakeOn; //display uart tx buffer
    bool auxBrakeOn;
    bool offroad;
};

extern BrakeData brakeData;

#endif // BRAKE_H