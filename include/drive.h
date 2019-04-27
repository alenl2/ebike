#ifdef __cplusplus
extern "C" {
#endif

#ifndef DRIVE_H
#define DRIVE_H


void drive_init();
void drive_update();

/*
struct DriveData {
    float throttleProcentual;
    int pwmValue;
};

extern DriveData driveData;
*/
#endif // DRIVE_H

#ifdef __cplusplus
}
#endif