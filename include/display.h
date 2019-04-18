#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>

void display_init();
void display_update();
void display_parse();

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
    bool cruise;
};

struct DisplayVariables
{
    uint8_t displaySerialBuffer[13]; //display uart rx buffer
    uint8_t ui8_assist_level;
    uint8_t ui8_max_speed;
    uint8_t ui8_wheel_size;
    uint8_t ui8_p1;
    uint8_t ui8_p2;
    uint8_t ui8_p3;
    uint8_t ui8_p4;
    uint8_t ui8_p5;
    uint8_t ui8_c1;
    uint8_t ui8_c2;
    uint8_t ui8_c4;
    uint8_t ui8_c5;
    uint8_t ui8_c12;
    uint8_t ui8_c13;
    uint8_t ui8_c14;
};

extern DisplayData displayData;
extern DisplayVariables displayVariables;

#endif // DISPLAY_H