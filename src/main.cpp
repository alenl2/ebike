#include "display.h"
#include "brake.h"
#include "key.h"
#include "throttle.h"
#include "pas.h"

HardwareSerial Pc(USART6); // uart 1

void debugLcd(){
  if(displayVariables.newData == false){
    return;
  }
  displayVariables.newData=false;
  Pc.print("P1:");
  Pc.print(displayVariables.ui8_p1);
  Pc.print(" ");

  Pc.print("P2:");
  Pc.print(displayVariables.ui8_p2);
  Pc.print(" ");

  Pc.print("P3:");
  Pc.print(displayVariables.ui8_p3);
  Pc.print(" ");
  
  Pc.print("P4:");
  Pc.print(displayVariables.ui8_p4);
  Pc.print(" ");

  Pc.print("P5:");
  Pc.print(displayVariables.ui8_p5);
  Pc.print(" ");

  Pc.print("C1:");
  Pc.print(displayVariables.ui8_c1);
  Pc.print(" ");

  Pc.print("C2:");
  Pc.print(displayVariables.ui8_c2);
  Pc.print(" ");

  Pc.print("C4:");
  Pc.print(displayVariables.ui8_c4);
  Pc.print(" ");

  Pc.print("C5:");
  Pc.print(displayVariables.ui8_c5);
  Pc.print(" ");

  Pc.print("C12:");
  Pc.print(displayVariables.ui8_c12);
  Pc.print(" ");

  Pc.print("C13:");
  Pc.print(displayVariables.ui8_c13);
  Pc.print(" ");

  Pc.print("C14:");
  Pc.print(displayVariables.ui8_c14);
  Pc.print(" ");

  Pc.print("Max spd:");
  Pc.print(displayVariables.ui8_max_speed);
  Pc.print(" ");

  Pc.print("Whell size:");
  Pc.print(displayVariables.ui8_wheel_size);
  Pc.print(" ");

  Pc.print("Assist:");
  Pc.print(displayVariables.ui8_assist_level);
  Pc.print(" ");

  Pc.print("Light:");
  Pc.print(displayVariables.lightOn);
  Pc.print(" ");

  Pc.print("Push assist:");
  Pc.print(displayVariables.walkAssistOn);
  Pc.print(" ");

  Pc.print("Cruise:");
  Pc.print(displayVariables.enterCruise);
  Pc.print(" ");

  Pc.print("RAW:");
  for(int i=0;i<13;i++){
    Pc.print(displayVariables.displaySerialBuffer[i], BIN);
    Pc.print(" ");
  }
  Pc.print("Calc crc:");
  Pc.print(displayVariables.calcCrc);
  Pc.print(" ");

  Pc.print("Report crc: ");
  Pc.print(displayVariables.reportedCrc);
  Pc.print(" ");

  Pc.println();
  Pc.flush();
}

void debugThrottle(){
  Pc.print(throttleData.throttleOn);
  Pc.print(" ");
  Pc.print(throttleData.procentual);
  Pc.print(" ");
  Pc.println(throttleData.throttleValue);
}

void debugBrake(){
  Pc.print("Main brake:");
  Pc.print(brakeData.brakeOn);
  Pc.print(" ");
  Pc.print("Aux brake:");
  Pc.println(brakeData.auxBrakeOn);
}

void debugKey(){
  Pc.print("Key:");
  Pc.println(keyData.keyOn);
}

/*
TODO 
read
  display light

write
  cadence number

*/
void setup() {
    Pc.begin(115200);
    display_init();
    brake_init();
    key_init();
    throttle_init();
    pas_init();

    displayData.error = None;
    displayData.batteryVoltage = 49;
    displayData.speed = 1000;
}
int count =0;
void loop() {
  count++;
  display_parse();
  display_update();

  brake_update();
  key_update();
  throttle_update();
  pas_update();

  debugLcd();

}


/*
TODO:
  LCD:
    READ:
        Test P and C values
    
    Write:
        Calibrate speedo
        Test cadence

*/