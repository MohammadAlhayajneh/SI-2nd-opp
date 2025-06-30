#include "led_timer.h"
#include "fsl_pit.h"
#include "fsl_gpio.h"
#include "fsl_lcd.h"
#include "BOARD/board.h"

#define PIT_BASE PIT
#define PIT_CHANNEL kPIT_Chnl_0

// Frequency table: 0 Hz (off), 0.5 Hz, 1 Hz, 2 Hz
static const uint32_t freq_period_ms[] = {0, 2000, 1000, 500};
static const char *freq_str[] = {"0 Hz", "0.5 Hz", "1 Hz", "2 Hz"};
#define FREQ_COUNT 4
static volatile uint8_t freq_index = 2; // Start at 1 Hz
static volatile uint8_t led_on = 0;

static void update_lcd(void) {
    LCD_ClearDisplay();
    LCD_WriteString((char*)freq_str[freq_index]);
}

void LED_Timer_Init(void) {
    // Init PIT
    PIT_Init(PIT_BASE);
    PIT_SetTimerPeriod(PIT_BASE, PIT_CHANNEL, USEC_TO_COUNT(freq_period_ms[freq_index] * 1000U, CLOCK_GetFreq(kCLOCK_BusClk)));
    PIT_EnableInterrupts(PIT_BASE, PIT_TFLG_TIF_MASK);
    NVIC_EnableIRQ(PIT_IRQn);
    PIT_StartTimer(PIT_BASE, PIT_CHANNEL);

    // Init green LED
    gpio_pin_config_t led_config = {kGPIO_DigitalOutput, 0};
    GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, &led_config);
    GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, 0);

    update_lcd();
}

void LED_Timer_ButtonLeft(void) {
    if (freq_index > 0) freq_index--;
    update_lcd();
    if (freq_index == 0) {
        GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, 1); // LED off
        PIT_StopTimer(PIT_BASE, PIT_CHANNEL);
    } else {
        PIT_SetTimerPeriod(PIT_BASE, PIT_CHANNEL, USEC_TO_COUNT(freq_period_ms[freq_index] * 1000U, CLOCK_GetFreq(kCLOCK_BusClk)));
        PIT_StartTimer(PIT_BASE, PIT_CHANNEL);
    }
}

void LED_Timer_ButtonRight(void) {
    if (freq_index < FREQ_COUNT - 1) freq_index++;
    update_lcd();
    if (freq_index == 0) {
        GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, 1); // LED off
        PIT_StopTimer(PIT_BASE, PIT_CHANNEL);
    } else {
        PIT_SetTimerPeriod(PIT_BASE, PIT_CHANNEL, USEC_TO_COUNT(freq_period_ms[freq_index] * 1000U, CLOCK_GetFreq(kCLOCK_BusClk)));
        PIT_StartTimer(PIT_BASE, PIT_CHANNEL);
    }
}

uint8_t LED_Timer_GetFreqIndex(void) {
    return freq_index;
}

void PIT_IRQHandler(void) {
    PIT_ClearStatusFlags(PIT_BASE, PIT_TFLG_TIF_MASK);
    if (freq_index == 0) return;
    led_on = !led_on;
    GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, !led_on); // active low
} 