#include "fsm_lock.h"
#include "fsl_gpio.h"
#include "board.h"

#define SEQ_LEN 4
static const char correct_sequence[SEQ_LEN] = {'A', 'B', 'A', 'A'};
static char input_sequence[SEQ_LEN];
static uint8_t input_index = 0;
static fsm_state_t state = FSM_LOCKED;

static void set_leds(fsm_state_t s) {
    // Red LED: PTB18, Green LED: PTB19 on FRDM-KL46Z
    // 0 = ON, 1 = OFF (LEDs are active low)
    switch (s) {
        case FSM_LOCKED:
            GPIO_PinWrite(GPIOB, 18U, 0); // Red ON
            GPIO_PinWrite(GPIOB, 19U, 1); // Green OFF
            break;
        case FSM_UNLOCKED:
            GPIO_PinWrite(GPIOB, 18U, 1); // Red OFF
            GPIO_PinWrite(GPIOB, 19U, 0); // Green ON
            break;
        case FSM_ENTERING:
            GPIO_PinWrite(GPIOB, 18U, 1); // Red OFF
            GPIO_PinWrite(GPIOB, 19U, 1); // Green OFF
            break;
    }
}

void FSM_Lock_Init(void) {
    input_index = 0;
    state = FSM_LOCKED;
    set_leds(state);
}

static void process_input(char btn) {
    if (state == FSM_UNLOCKED) return;
    input_sequence[input_index++] = btn;
    state = FSM_ENTERING;
    set_leds(state);

    if (input_index == SEQ_LEN) {
        int match = 1;
        for (uint8_t i = 0; i < SEQ_LEN; ++i) {
            if (input_sequence[i] != correct_sequence[i]) {
                match = 0;
                break;
            }
        }
        if (match) {
            state = FSM_UNLOCKED;
        } else {
            state = FSM_LOCKED;
        }
        input_index = 0;
        set_leds(state);
    }
}

void FSM_Lock_ButtonA(void) {
    process_input('A');
}

void FSM_Lock_ButtonB(void) {
    process_input('B');
}

fsm_state_t FSM_Lock_GetState(void) {
    return state;
} 