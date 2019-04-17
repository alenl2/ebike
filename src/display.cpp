#include "display.h"

DisplayData displayData;

uint8_t ui8_j; //conter for for loop
uint8_t ui8_crc; //crc calculation
uint8_t ui8_moving_indication; //brake throttle and pas indicators
uint16_t ui16_wheel_period_ms;
uint8_t ui8_battery_soc;

void display_init() {
    displayData.batteryBarCount = 4; //4 full 0 empty
    displayData.batteryVoltage = 24; // voltage
    displayData.speed = 10; //time for one wheel rotation
    displayData.wattage = 255;
    displayData.temperature = 20;
    displayData.brake = false;
    displayData.throttle = false;
    displayData.pas = false;
}

void display_update() {

	ui16_wheel_period_ms = displayData.speed * 150;


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
	//if (ebike_app_cruise_control_is_set ()) { ui8_moving_indication |= (1 << 3); }
	//if (throttle_is_set ()) { ui8_moving_indication |= (1 << 1); }
	//if (pas_is_set ()) { ui8_moving_indication |= (1 << 4); }

	displayData.displaySerialBuffer [0] = 65; // header

	// B1: battery level
	displayData.displaySerialBuffer [1] = ui8_battery_soc;
	// B2: 24V controller
	displayData.displaySerialBuffer [2] = displayData.batteryVoltage;
	// B3: speed, wheel rotation period, ms; period(ms)=B3*256+B4;
	displayData.displaySerialBuffer [3] = (ui16_wheel_period_ms >> 8) & 0xff;
	displayData.displaySerialBuffer [4] = ui16_wheel_period_ms & 0xff;

	// B5: error info display
	displayData.displaySerialBuffer [5] = 0;

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
	displayData.displaySerialBuffer [8] = displayData.wattage;

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
}