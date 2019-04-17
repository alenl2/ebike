#include "display.h"

uint8_t  buffCounter;
int incomingByte = 0;


void setup() {
    Serial.begin(9600);
    display_init();
}

void loop() {

  display_update();

	for (int buffCounter = 0; buffCounter <= 11; buffCounter++) {
		Serial.write(displayData.displaySerialBuffer[buffCounter]);
	}

  if (Serial.available() > 0) {
        // read the incoming byte:
        incomingByte = Serial.read();

        // say what you got:
        Serial.print("I received: ");
        Serial.println(incomingByte, DEC);
  }
}