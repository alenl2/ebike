#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>

void drive_init();
void drive_update();

struct DriveData {
    uint8_t driveSerialDataBuffer[12]; //display uart tx buffer

};

extern DriveData driveData;

#endif // DISPLAY_H