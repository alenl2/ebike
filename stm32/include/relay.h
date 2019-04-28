#ifndef RELAY_H
#define RELAY_H
#include <Arduino.h>

void relay_init();
void relay_update();

struct RelayData {
    bool relay1On;
};

extern RelayData relayData;

#endif // RELAY_H