// Author: Aananth C N
// Date: 2 July 2021, 7:20 PM

#include <ostypes.h>

/*
 * BLE Carbon board has 2 LEDs
 * 1. USR1 - PD2
 * 2. USR2 - PA15
 */
#define GPIO_PORT_A_BASE	0x40020000
#define GPIO_PORT_D_BASE	0x40020D00

#define GPIO_MODE_R_OFFSET  0x0
#define GPIO_INPUTR_OFFSET  0x10
#define GPIO_OUTPUT_OFFSET  0x14

#define USR1_LED_PIN    2
#define USR2_LED_PIN    15

#define RCC_AHB1ENR_BASE    0x40023800
#define RCC_AHB1ENR_OFFSET  0x30
#define GPIODEN_BIT_OFFSET  3


void user_led_init(void)
{
    u32 tmp;

    /* Enable GPIO_D clock*/
    tmp = *((u32*)(RCC_AHB1ENR_BASE+RCC_AHB1ENR_OFFSET));
    tmp |= (1 << GPIODEN_BIT_OFFSET);
    *((u32*)(RCC_AHB1ENR_BASE+RCC_AHB1ENR_OFFSET)) = tmp;

	/* Configure Port D, 2nd pin as GP Output Mode for USR1 LED */
    tmp = *((u32*)(GPIO_PORT_D_BASE+GPIO_MODE_R_OFFSET));
    tmp |= 1 << (USR1_LED_PIN * 2);
    *((u32*)(GPIO_PORT_D_BASE+GPIO_MODE_R_OFFSET)) = tmp;
}

bool get_user_led1(void)
{
    u16 data = *((u16*)(GPIO_PORT_D_BASE+GPIO_INPUTR_OFFSET));
    if (data & (1 << USR1_LED_PIN))
        return true;
    else
        return false;
}

void set_user_led1(bool val)
{
    u16 data = *((u16*)(GPIO_PORT_D_BASE+GPIO_INPUTR_OFFSET));
    if (val)
        data |= (1 << USR1_LED_PIN);
    else
        data &= ~(1 << USR1_LED_PIN);
    
    *((u16*)(GPIO_PORT_D_BASE+GPIO_OUTPUT_OFFSET)) = data;
}

void toggle_user_led1(void)
{
    if (get_user_led1())
        set_user_led1(false);
    else
        set_user_led1(true);
}