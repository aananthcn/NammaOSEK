#include <ostypes.h>
#include <stdarg.h>
#include <stdio.h>

#include <osek.h>
#include <sg_appmodes.h>
#include <sg_counter.h>

#include <os_api.h>


#include "rp2040.h"


/* Serial console functions */
int console_fputc(const int c) {
#if 0
        while((USART1_SR & 0x40) == 0); // wait until TC = 1
        USART1_DR = (unsigned int) (c & 0xFF);
#endif
        return c;
}

int console_fputs(const char *s) {
        int count = 0;

        while (*s != '\0') {
                console_fputc(*s);
                count++;
                s++;
        }

        return count;
}



#define CLOCK_SEC2MSEC          (1000) /* 1000ms = 1 sec */ 

/* SysTick clk = System Clock = 8 MHz (FCLK Cortex free-running clock) */
#define SYSTEM_CLOCK_MHz        (8)

int brd_setup_systimer(void) {
        u32 tick_count = OS_TICK_DURATION_ns * SYSTEM_CLOCK_MHz / CLOCK_SEC2MSEC;

        /* Setup Clocks and Oscillator */
        RCC_PLLCFGR = 0x02003010; /* Q = 2,  P = 2(0), N = 192, M = 16 */
        RCC_CFGR |= 0x01; /* SW (System Clock) = HSE clock; AHB & APB uses this clock */
        RCC_CR |= ((1 << 24) | (1 << 16)); /* PLL = ON, HSE = ON; Ext. Osc. 8 MHz */

        /* Using SysTick Timer to generate system ticks */
        CM4_SYSTICK_STRVR = tick_count & 0x00FFFFFF;
        CM4_SYSTICK_STCVR = 0;
        CM4_SYSTICK_STCSR = 0x7; /* Core Clock | TICKINT | Down Counter */

        /* Prepare Timer 2 for free-running up-counter */
        RCC_APB1ENR |= 0x01; /* Enable TIM2 Clock */
        RCC_APB2ENR |= ((1 << 14) | (1 << 4)); /* SYSCFGEN = 1 to access APB1 bus; USART1EN */

        /* Using TIM2 for free-running 32-bit counter */
        TIM2_PSC = 7; /* f = fCK_PSC / (PSC[15:0] + 1) = 8 MHz / (7 + 1) = 1 MHz */
        TIM2_CR2 = 0x00;
        TIM2_EGR = 0x01; /* If UG bit is not set, the prescalar (PSC) has no effect */
        TIM2_CR1 = 0x01; /* Enabling the Timer */


        return 0;
}

int brd_get_usec_syscount(u32 *ucount) {
        u32 count;

        *ucount = TIM2_CNT; /* Runs at 1 MHz, hence 1 count == 1 µs */

        return 0;
}


int brd_sys_enable_interrupts() {
#if 0
        /* Enable interrupts */
        VIC_INTENABLE = 1 << ISR_SN_TIMER01;
#endif

        return 0;
}


int brd_console_init(void) {
        /* USART1 is mapped to Port A, so, enable peripheral clock to GPIO-A */
        RCC_AHB1ENR |= (0x00000001);
        /* Configure GPIOs as Alt. Fun. for Tx (PA9) & Rx (PA10) pins */
        GPIOA_MODER |= (0x00280000);
        /* Configure Alt. Fun. = AF7 for UART for Tx (PA9) & Rx (PA10) pins */
        GPIOA_AFRH  |= (0x00000770);

        /* First enable the USART module before you configure it */
        USART1_CR1 |= (1 << 13); /* Set UE */

        /* PCLK = 8 MHz, 16 samples per bit = 500 kHz, BR = 500k/115200 = 4.34 */
        USART1_BRR |= ((4) << 4 | (5)); /* Mantissa = 4, Fraction = 5 / 16 = 0.3125 */
        /* Set prescalar to div by 1 */
        USART1_GTPR  |= (0x00000001);

        /* Now enable transmission and reception */
        USART1_CR1 |= ((1 << 3) | (1 << 2)); /* TE | RE */

        pr_log_init();

        return 0;
}


void main(void) {
        brd_setup_systimer();
        brd_console_init();
        brd_sys_enable_interrupts();

        StartOS(OSDEFAULTAPPMODE);
        /* The execution should never reach here */
        pr_log("Info: StartOS() function returned!! OS Exits!\n");
}
