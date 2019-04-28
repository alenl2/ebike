#include "relay.h"

RelayData relayData;
int relayPin = PC8;
bool relayPinOn = false;

void relay_init() {
    relayData.relay1On = relayPinOn;
    pinMode(relayPin, OUTPUT);
}


void relay_update() {
    if(relayPin != relayData.relay1On){
        if(relayData.relay1On){
            digitalWrite(relayPin, HIGH);
        }else{
            digitalWrite(relayPin, LOW);
        }
    }
    
}