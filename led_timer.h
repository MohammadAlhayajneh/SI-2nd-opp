#ifndef LED_TIMER_H
#define LED_TIMER_H

#include <stdint.h>

void LED_Timer_Init(void);
void LED_Timer_ButtonLeft(void);
void LED_Timer_ButtonRight(void);
uint8_t LED_Timer_GetFreqIndex(void);

#endif // LED_TIMER_H 