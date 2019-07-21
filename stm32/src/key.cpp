#include "key.h"

KeyData keyData;
int keyPin = PB2;

void key_init() {
    keyData.keyOn = false;
    pinMode(keyPin, INPUT_PULLUP);
}

void key_update() {
    if(digitalRead(keyPin)){
        keyData.keyOn = false;
    }else{
       keyData.keyOn = true;
    }
}