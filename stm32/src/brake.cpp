#include "brake.h"

BrakeData brakeData;
int brakePin = PC4;
int auxBrake = PA5;

void brake_init() {
    brakeData.brakeOn = false;
    brakeData.auxBrakeOn = false;
    pinMode(brakePin, INPUT_PULLUP);
    pinMode(auxBrake, INPUT_PULLUP);

    if(digitalRead(auxBrake)){
         brakeData.offroad = false;
    }else{
       brakeData.offroad = true;
    }
}


void brake_update() {
    if(digitalRead(brakePin)){
         brakeData.brakeOn = false;
    }else{
       brakeData.brakeOn = true;
    }

    if(digitalRead(auxBrake)){
         brakeData.auxBrakeOn = false;
    }else{
       brakeData.auxBrakeOn = true;
    }
}