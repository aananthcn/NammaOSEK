#include <ostypes.h>
#include <stdarg.h>
#include <stdio.h>

#include <osek.h>
#include <sg_appmodes.h>
#include <sg_counter.h>

#include <os_api.h>


#include "stm32f407vet6.h"


/* Serial console functions */
int console_fputs(const char *s) {
        int count = 0;

        while (*s != '\0') {
                #if 0
                UART0DR = (unsigned int)(*s);
                #endif
                count++;
                s++;
        }

        return count;
}

int console_fputc(const int c) {
#if 0
        UART0DR = (unsigned int) (c);
#endif
        return c;
}




#define CLOCK_SEC2MSEC          (1000) /* 1000ms = 1 sec */ 
/* SysTick clk = System Clock / 8 = 24 MHz / 8 = 3 MHz */
#define SYSTEM_CLOCK_MHz        (3)
int brd_setup_sytimer(void) {
        u32 tick_count = OS_TICK_DURATION_ns * SYSTEM_CLOCK_MHz / CLOCK_SEC2MSEC;

        /* Setup Clocks and Oscillator */
        RCC_PLLCFGR = 0x24003010; /* HSE Clk, PLL clock = 3 * 16 MHz / 2 = 24 MHz */
        RCC_CFGR = 0x02; /* SW (System Clock) = PLL clock; AHB & APB uses the PLL Clock */
        RCC_CR = 0x10000; /* HSE = ON, Ext. Osc. 16 MHz */

        /* Using SysTick Timer to generate system ticks */
        CM4_SYSTICK_STRVR = tick_count & 0x00FFFFFF;
        CM4_SYSTICK_STCVR = 0;
        CM4_SYSTICK_STCSR = 0x7; /* Core Clock | TICKINT | Down Counter */

        /* Prepare Timer 2 for free-running up-counter */
        RCC_APB1RSTR = 0x01; /* Reset TIM2 */
        RCC_APB1ENR = 0x01; /* Enable TIM2 Clock */

        /* Using TIM2 for free-running 32-bit counter */
        TIM2_PSC = 23; /* f = fCK_PSC / (PSC[15:0] + 1) = 24 MHz / (23 + 1) = 1 MHz */
        TIM2_CR2 = 0x00;
        TIM2_CR1 = 0x01; /* Enabling the Timer */


        return 0;
}

int brd_get_usec_syscount(u32 *ucount) {
        u32 count;

#if 0
        /* free running mode counts from 0xFFFFFFFF to 0, hence reversing it */
        count = (u32)0xFFFFFFFF - *((volatile u32*)(TIMER1_BASE+TIMERVALUE_OFFSET));

        /* convert to ucount so that the count value == 1 usec */
        *ucount = (u32)((count * 1000ull) / TIMER_CLK_TO_MILLISEC);
        //*ucount = count;
#endif
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


void main(void) {
        pr_log_init();
        brd_setup_sytimer();
        brd_sys_enable_interrupts();

        StartOS(OSDEFAULTAPPMODE);
        /* The execution should never reach here */
        pr_log("Info: StartOS() function returned!! OS Exits!\n");
}
