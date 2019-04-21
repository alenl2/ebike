#ifndef KEY_H
#define KEY_H
#include <Arduino.h>

void key_init();
void key_update();

struct KeyData {
    bool keyOn; //display uart tx buffer

};

extern KeyData keyData;

#endif // KEY_H