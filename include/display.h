#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>

void display_init();
void display_update();

struct DisplayData {
    uint8_t displaySerialBuffer[12]; //display uart tx buffer
    uint8_t batteryBarCount; //4 full 0 empty
    uint8_t batteryVoltage; // voltage
    uint8_t speed; //time for one wheel rotation
    uint8_t  wattage;
    uint8_t  temperature;
    bool brake;
    bool throttle;
    bool pas;
};

extern DisplayData displayData;

#endif // DISPLAY_H