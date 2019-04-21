#include "pas.h"

PasData pasData;

int pasPin = PA4;

void blink() {
  pasData.pasSpeed++;
}

void pas_init() {
    pinMode(pasPin, INPUT_PULLUP);
    attachInterrupt(pasPin, blink, CHANGE);
}

void pas_update() {


}

