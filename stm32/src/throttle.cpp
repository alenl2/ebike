#include "throttle.h"

ThrottleData throttleData;

int throttlePin = PB0;

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void throttle_init() {
    throttleData.throttleOn = false;
    throttleData.minValue = 310;
    throttleData.maxValue = 803;
    throttleData.throttleValue = 0;
}

void throttle_update() {

    throttleData.throttleValue = analogRead(throttlePin);
    if(throttleData.throttleValue > throttleData.minValue){
        throttleData.procentual = mapfloat(throttleData.throttleValue, throttleData.minValue, throttleData.maxValue,0.0f,100.0f);
        if(throttleData.procentual >= 100){
            throttleData.procentual= 99;
        }
        throttleData.throttleOn = true;
    }else{
        throttleData.procentual = 0;
        throttleData.throttleOn = false;
    }
}

