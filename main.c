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

int main(void)
{
  char ch;

  /* Init board hardware. */
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();

  // Enable clock for button and LED ports
  CLOCK_EnableClock(kCLOCK_PortC);
  CLOCK_EnableClock(kCLOCK_PortB);

  // Configure button pins as GPIO with interrupt
  gpio_pin_config_t btn_config = {kGPIO_DigitalInput, 0};
  GPIO_PinInit(BOARD_SW1_GPIO, BOARD_SW1_GPIO_PIN, &btn_config); // SW1 (left)
  GPIO_PinInit(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN, &btn_config); // SW3 (right)

  // Configure interrupts for falling edge (button press)
  PORT_SetPinInterruptConfig(BOARD_SW1_PORT, BOARD_SW1_GPIO_PIN, kPORT_InterruptFallingEdge);
  PORT_SetPinInterruptConfig(BOARD_SW3_PORT, BOARD_SW3_GPIO_PIN, kPORT_InterruptFallingEdge);

  // Enable NVIC for PORTC
  NVIC_EnableIRQ(BOARD_SW1_IRQ);

  // Initialize LED timer system
  LED_Timer_Init();

  while (1)
    {
      ch = GETCHAR();
      PUTCHAR(ch);
    }
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
