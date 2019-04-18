#include "display.h"




void debugSerial(){
  Serial.print("P1:");
  Serial.print(displayVariables.ui8_p1);
  Serial.print(" ");

  Serial.print("P2:");
  Serial.print(displayVariables.ui8_p2);
  Serial.print(" ");

  Serial.print("P3:");
  Serial.print(displayVariables.ui8_p3);
  Serial.print(" ");
  
  Serial.print("P4:");
  Serial.print(displayVariables.ui8_p4);
  Serial.print(" ");

  Serial.print("P5:");
  Serial.print(displayVariables.ui8_p5);
  Serial.print(" ");

  Serial.print("C1:");
  Serial.print(displayVariables.ui8_c1);
  Serial.print(" ");

  Serial.print("C2:");
  Serial.print(displayVariables.ui8_c2);
  Serial.print(" ");

  Serial.print("C4:");
  Serial.print(displayVariables.ui8_c4);
  Serial.print(" ");

  Serial.print("C5:");
  Serial.print(displayVariables.ui8_c5);
  Serial.print(" ");

  Serial.print("C12:");
  Serial.print(displayVariables.ui8_c12);
  Serial.print(" ");

  Serial.print("C13:");
  Serial.print(displayVariables.ui8_c13);
  Serial.print(" ");

  Serial.print("C14:");
  Serial.print(displayVariables.ui8_c14);
  Serial.print(" ");

  Serial.print("Max spd:");
  Serial.print(displayVariables.ui8_max_speed);
  Serial.print(" ");

  Serial.print("Whell size:");
  Serial.print(displayVariables.ui8_wheel_size);
  Serial.print(" ");

  Serial.print("Assist:");
  Serial.print(displayVariables.ui8_assist_level);
  Serial.print(" ");

  Serial.print("RAW:");
  for(int i=0;i<13;i++){
    Serial.print(displayVariables.displaySerialBuffer[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  Serial.flush();
}

/*
TODO 
read
  display light

write
  cadence number

*/
void setup() {
    Serial.begin(115200);
    display_init();

    displayData.batteryBarCount = 4; //4 full 0 empty
    displayData.batteryVoltage = 40; // voltage
    displayData.speed = 8500; //time for one wheel rotation  //10=4.3 //20=2.2
    displayData.wattage = 255;
    displayData.temperature = 20;
    displayData.brake = true;
    displayData.throttle = true;
    displayData.pas = true;
    displayData.cruise = true;
}

void loop() {
  display_parse();

  display_update();
}
