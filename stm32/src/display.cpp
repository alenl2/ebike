#include "display.h"

DisplayData displayData;
DisplayVariables displayVariables;

HardwareSerial Display(USART1); // uart 1

uint8_t ui8_j; //conter for for loop
uint8_t ui8_crc; //crc calculation
uint8_t ui8_moving_indication; //brake throttle and pas indicators
uint16_t ui16_wheel_period_ms;
uint8_t ui8_battery_soc;
uint32_t lastOkRx;

bool rdyToTransmit;

void display_init() {
	Display.begin(9600, SERIAL_8N1);

	rdyToTransmit = false;
	displayData.speed = 0; //time for one wheel rotation  //10=4.3 //20=2.2
    displayData.watts = 0;
	lastOkRx = 0;

    displayData.batteryBarCount = 0; //4 full 0 empty
    displayData.batteryVoltage = 12 * 3.45; // voltage
    displayData.temperature = 0;
    displayData.brake = false;
    displayData.throttle = false;
    displayData.pas = false;
    displayData.error = Info1;
    displayData.cruise = false;
	displayVariables.didInit = false;

	
}
//displayData.speed
void display_update() {
	if(rdyToTransmit == false){
		return;
	}

	if(displayVariables.didInit == false){
		return;
	}
	rdyToTransmit = false;

	ui16_wheel_period_ms = displayData.speed;

	if (displayData.batteryBarCount == 4) {
		ui8_battery_soc = 16; // 4 bars | full
	}
	else if (displayData.batteryBarCount == 3) {
		ui8_battery_soc = 12; // 3 bars
	}
	else if (displayData.batteryBarCount == 2) {
		ui8_battery_soc = 8; // 2 bars
	}
	else if (displayData.batteryBarCount == 1) {
		ui8_battery_soc = 4; // 1 bar
	}
	else {
		ui8_battery_soc = 3; // empty
	}



    ui8_moving_indication = 0;
	if (displayData.brake) {
		ui8_moving_indication |= (1 << 5);
	}
	if (displayData.cruise) { ui8_moving_indication |= (1 << 3); }
	if (displayData.throttle) { ui8_moving_indication |= (1 << 1); }
	if (displayData.pas) { ui8_moving_indication |= (1 << 4); }

	displayData.displaySerialBuffer [0] = 65; // header

	// B1: battery level
	displayData.displaySerialBuffer [1] = ui8_battery_soc;
	// B2: 24V controller
	displayData.displaySerialBuffer [2] = displayData.batteryVoltage;
	// B3: speed, wheel rotation period, ms; period(ms)=B3*256+B4;
	displayData.displaySerialBuffer [3] = (ui16_wheel_period_ms >> 8) & 0xff;
	displayData.displaySerialBuffer [4] = ui16_wheel_period_ms & 0xff;

	// B5: error info display
	displayData.displaySerialBuffer [5] = displayData.error;

  // B6: CRC: xor B1,B2,B3,B4,B5,B7,B8,B9,B10,B11
	displayData.displaySerialBuffer [6] = 0; 

	// B7: moving mode indication, bit throttle: 2
	displayData.displaySerialBuffer [7] = ui8_moving_indication;


	// B8: 4x controller current
	// Vbat = 30V:
	// - B8 = 255, LCD shows 1912 watts
	// - B8 = 250, LCD shows 1875 watts
	// - B8 = 100, LCD shows 750 watts
	// each unit of B8 = 0.25A
	displayData.displaySerialBuffer [8] = displayData.watts / displayData.batteryVoltage * 4;

	// B9: motor temperature
	displayData.displaySerialBuffer [9] = displayData.temperature - 15; //according to documentation at endless sphere

	// B10 and B11: 0
	displayData.displaySerialBuffer [10] = 0;
	displayData.displaySerialBuffer [11] = 0;

	// calculate CRC xor
	ui8_crc = 0;
	for (ui8_j = 1; ui8_j <= 11; ui8_j++) {
		ui8_crc ^= displayData.displaySerialBuffer[ui8_j];
	}
	displayData.displaySerialBuffer [6] = ui8_crc;

	for(int i=0;i<13;i++){
		Display.write(displayData.displaySerialBuffer[i]);
	}
}

void display_parse(){

	if(lastOkRx+5000 > millis()){
		// after 5sec of no data deinit display
		displayVariables.didInit = false;
	}
  
  static boolean recvInProgress = false;
  static byte ndx = 0;         //
  uint8_t  startInt = 14;          // 128 is integer equivalent of 1000 0000 <- start bit
  uint8_t  endInt = 14;             // 64 is integer equivalent of 0100 0000 <- stop bit
  uint8_t ri;                      //
  
  while (Display.available() > 0) {
   ri = Display.read();

   if (recvInProgress == true) {
     if (ri != endInt) {
       displayVariables.displaySerialBuffer[ndx] = ri;
       ndx++;
       if (ndx >= 13) {
         ndx = 13 - 1;
       }
     } else {
       //displayVariables.displaySerialBuffer[ndx] = '\0'; // terminate the string
       recvInProgress = false;
       ndx = 0;

		ui8_crc = 0;
		for (ui8_j = 0; ui8_j < 13; ui8_j++) {
			
			if (ui8_j == 5) continue; // don't xor B5 
			ui8_crc ^= displayVariables.displaySerialBuffer[ui8_j];
		}
		ui8_crc-=12;

		displayVariables.calcCrc = ui8_crc;
		displayVariables.reportedCrc = displayVariables.displaySerialBuffer [5];
/*
		
		// see if CRC is ok
		if (((ui8_crc ^ 10) == displayVariables.displaySerialBuffer [5]) || // some versions of CRC LCD5 (??)
				((ui8_crc ^ 1) == displayVariables.displaySerialBuffer [5]) || // CRC LCD3 (tested with KT36/48SVPR, from PSWpower)
				((ui8_crc ^ 2) == displayVariables.displaySerialBuffer [5]) || // CRC LCD5
				((ui8_crc ^ 3) == displayVariables.displaySerialBuffer [5]) || // CRC LCD5 Added display 5 Romanta
				((ui8_crc ^ 4) == displayVariables.displaySerialBuffer [5]) ||
				((ui8_crc ^ 5) == displayVariables.displaySerialBuffer [5]) ||
				((ui8_crc ^ 6) == displayVariables.displaySerialBuffer [5]) ||
				((ui8_crc ^ 7) == displayVariables.displaySerialBuffer [5]) ||
				((ui8_crc ^ 8) == displayVariables.displaySerialBuffer [5]) ||
				((ui8_crc ^ 9) == displayVariables.displaySerialBuffer [5])) // CRC LCD3
		{
			*/
			

			if(displayVariables.displaySerialBuffer [1] & 7 == 6){
				displayVariables.walkAssistOn = true;
			}else{
				displayVariables.walkAssistOn = false;
				displayVariables.ui8_assist_level = displayVariables.displaySerialBuffer [1] & 7;
			}
			if (  displayVariables.displaySerialBuffer [1] & (1 << 7) )
			{
				displayVariables.lightOn = true;
			}else{
				displayVariables.lightOn = false;
			}

			if (  displayVariables.displaySerialBuffer [8] & (1 << 4) )
			{
				displayVariables.enterCruise = true;
			}else{
				displayVariables.enterCruise = false;
			}


			displayVariables.ui8_max_speed = 10 + ((displayVariables.displaySerialBuffer [2] & 248) >> 3) | (displayVariables.displaySerialBuffer [4] & 32);
			displayVariables.ui8_wheel_size = ((displayVariables.displaySerialBuffer [4] & 192) >> 6) | ((displayVariables.displaySerialBuffer [2] & 7) << 2);

			displayVariables.ui8_p1 = displayVariables.displaySerialBuffer[3];
			displayVariables.ui8_p2 = displayVariables.displaySerialBuffer[4] & 0x07;
			displayVariables.ui8_p3 = displayVariables.displaySerialBuffer[4] & 0x08;
			displayVariables.ui8_p4 = displayVariables.displaySerialBuffer[4] & 0x10;
			displayVariables.ui8_p5 = displayVariables.displaySerialBuffer[0];

			displayVariables.ui8_c1 = (displayVariables.displaySerialBuffer[6] & 0x38) >> 3;
			displayVariables.ui8_c2 = (displayVariables.displaySerialBuffer[6] & 0x37);
			displayVariables.ui8_c4 = (displayVariables.displaySerialBuffer[8] & 0xE0) >> 5;
			displayVariables.ui8_c5 = (displayVariables.displaySerialBuffer[7] & 0x0F);
			displayVariables.ui8_c12 = (displayVariables.displaySerialBuffer[9] & 0x0F);
			displayVariables.ui8_c13 = (displayVariables.displaySerialBuffer[10] & 0x1C) >> 2;
			displayVariables.ui8_c14 = (displayVariables.displaySerialBuffer[7] & 0x60) >> 5;
			displayVariables.newData = true;
			displayVariables.didInit = true;

			rdyToTransmit = true;
			lastOkRx = millis();
		//}




     }
    } else if (ri == startInt) {
      recvInProgress = true;
    }
  }

}

