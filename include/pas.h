#ifndef PAS_H
#define PAS_H
#include <Arduino.h>

void pas_init();
void pas_update();

struct PasData {
    int pasOn;
    int pasSpeed;
};

extern PasData pasData;

#endif // PAS_H