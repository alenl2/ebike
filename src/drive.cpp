#include "drive.h"



void display_init() {

}

void display_update() {


	driveData.driveSerialDataBuffer [0] = 65; // header

	// B1: battery level
	driveData.driveSerialDataBuffer [1] = ui8_battery_soc;
	// B2: 24V controller
  driveData.driveSerialDataBuffer [2] = displayData.batteryVoltage;
	// B3: speed, wheel rotation period, ms; period(ms)=B3*256+B4;
	driveData.driveSerialDataBuffer [3] = (ui16_wheel_period_ms >> 8) & 0xff;
	driveData.driveSerialDataBuffer [4] = ui16_wheel_period_ms & 0xff;

	// B5: error info display
	driveData.driveSerialDataBuffer [5] = 0;

  // B6: CRC: xor B1,B2,B3,B4,B5,B7,B8,B9,B10,B11
	ddriveData.driveSerialDataBuffer [6] = 0; 

	// B7: moving mode indication, bit throttle: 2
	driveData.driveSerialDataBuffer [7] = ui8_moving_indication;


	// B8: 4x controller current
	// Vbat = 30V:
	// - B8 = 255, LCD shows 1912 watts
	// - B8 = 250, LCD shows 1875 watts
	// - B8 = 100, LCD shows 750 watts
	// each unit of B8 = 0.25A
	driveData.driveSerialDataBuffer [8] = displayData.wattage;

	// B9: motor temperature
	driveData.driveSerialDataBuffer [9] = displayData.temperature - 15; //according to documentation at endless sphere

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