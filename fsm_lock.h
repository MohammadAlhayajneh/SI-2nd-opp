#ifndef FSM_LOCK_H
#define FSM_LOCK_H

#include <stdint.h>

typedef enum {
    FSM_LOCKED,
    FSM_UNLOCKED,
    FSM_ENTERING
} fsm_state_t;

void FSM_Lock_Init(void);
void FSM_Lock_ButtonA(void);
void FSM_Lock_ButtonB(void);
fsm_state_t FSM_Lock_GetState(void);

#endif // FSM_LOCK_H 