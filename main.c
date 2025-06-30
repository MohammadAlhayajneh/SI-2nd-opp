/*
 * The Clear BSD License
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "BOARD/board.h"
#include "pin_mux.h"
#include "fsm_lock.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "led_timer.h"
#include "countones.h"
#include "MKL46Z4.h"

int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    uint32_t test_val = 1554098974U; // 0x5ca1ab1e
    uint32_t resultA, resultB, resultC;
    uint32_t ticksA, ticksB, ticksC;

    // SysTick setup
    SysTick->LOAD = 0xFFFFFF;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    PRINTF("Benchmarking the count_ones routine:\r\n");
    PRINTF("32-bit integer used as argument: %u\r\n", test_val);

    // A
    SysTick->VAL = 0;
    resultA = count_ones_A(test_val);
    ticksA = 0xFFFFFF - SysTick->VAL;

    // B
    SysTick->VAL = 0;
    resultB = count_ones_B(test_val);
    ticksB = 0xFFFFFF - SysTick->VAL;

    // C
    SysTick->VAL = 0;
    resultC = count_ones_C(test_val);
    ticksC = 0xFFFFFF - SysTick->VAL;

    PRINTF("Elapsed ticks with count_ones_A(): %lu (Function result: %lu)\r\n", ticksA, resultA);
    PRINTF("Elapsed ticks with count_ones_B(): %lu (Function result: %lu)\r\n", ticksB, resultB);
    PRINTF("Elapsed ticks with count_ones_C(): %lu (Function result: %lu)\r\n", ticksC, resultC);

    while (1) {}
}

void BOARD_SW1_IRQ_HANDLER(void) {
    uint32_t flags = GPIO_PortGetInterruptFlags(BOARD_SW1_GPIO);
    if (flags & (1U << BOARD_SW1_GPIO_PIN)) {
        GPIO_PortClearInterruptFlags(BOARD_SW1_GPIO, 1U << BOARD_SW1_GPIO_PIN);
        LED_Timer_ButtonLeft();
    }
    if (flags & (1U << BOARD_SW3_GPIO_PIN)) {
        GPIO_PortClearInterruptFlags(BOARD_SW3_GPIO, 1U << BOARD_SW3_GPIO_PIN);
        LED_Timer_ButtonRight();
    }
}
