#ifndef DRIVE_H
#define DRIVE_H
#include <Arduino.h>

void drive_init();
void drive_update();


struct DriveData {
    float throttleProcentual;
    int pwmValue;
};

extern DriveData driveData;

#endif // DRIVE_H
