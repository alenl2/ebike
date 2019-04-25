#include "pas.h"

#define TIMER_BLINK  TIM4

PasData pasData;

int pasPin = PA4;

int pasThreashold = 5;
int timerSec = 2.1;

uint32_t lastUpdate;
static stimer_t TimHandle;

static void timer(stimer_t *htim) {
  UNUSED(htim);
  if(pasData.pasSpeed > pasThreashold){
    pasData.pasOn = true;
  }else{
    pasData.pasOn = false;
  }
  pasData.pasSpeed = 0;
}

void blink() {
  pasData.pasSpeed++;
}

void pas_init() {
  pinMode(pasPin, INPUT_PULLUP);
  attachInterrupt(pasPin, blink, CHANGE);
    
  TimHandle.timer = TIMER_BLINK;

  float timerSec = 1.5;
  TimerHandleInit(&TimHandle, timerSec*5000 - 1, ((uint32_t)(getTimerClkFreq(TIMER_BLINK) / (1000)) - 1));
  attachIntHandle(&TimHandle, timer);

}
void pas_update() {

}

