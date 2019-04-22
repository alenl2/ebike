#include "drive.h"
#include <QX_Parsing_Functions.h>


DriveData driveData;
int pwmPin = PB14;

HardwareSerial Drive(USART6); // uart 1

void drive_init() {
    pinMode(pwmPin, OUTPUT);  // sets the pin as output
    Drive.begin(115200, SERIAL_8N1);

    uint16_t command = 1010;
    uint8_t arcData[13];

    float dirveVar = 1.1f;

    //header
    arcData[0] = 0x51;
    arcData[1] = 0x58;

    //length
    arcData[2] = 9;

    //command
    arcData[3] = command & 0xff;
    arcData[4] = (command >> 8);

    //hardcoded
    arcData[5]=0x42;
    arcData[6]=0x0;
    arcData[7]=0x0;

    //command
    arcData[8]=0x0; //motor id

    union {
        float fval;
        byte bval[4];
    } floatAsBytes;

    floatAsBytes.fval = dirveVar;

    //command data
    arcData[9] = floatAsBytes.bval[0];
    arcData[10]= floatAsBytes.bval[1];
    arcData[11]= floatAsBytes.bval[2];
    arcData[12]= floatAsBytes.bval[3];

    uint8_t sum = 0;
    for(int i=3;i<12;i++){
        sum+=arcData[i];
    }
    arcData[13] = 255 - (sum % 256);
}

void drive_update() {
    if(driveData.throttleProcentual > 0){
        analogWrite(pwmPin, (driveData.throttleProcentual+100.0f));
    }else{
        digitalWrite(pwmPin, 0);
    }

    Drive.println(driveData.throttleProcentual+100.0f);
}