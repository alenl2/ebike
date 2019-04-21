#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>

void drive_init();
void drive_update();

struct DriveData {
};

extern DriveData driveData;

#endif // DISPLAY_H