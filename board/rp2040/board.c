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

/* SysTick clk = External Clock = 12 MHz; let us not use 125MHz System clock for SysTick */
#define SYSTICK_CLOCK_MHz       (12)

int brd_setup_systimer(void) {
        register u32 tick_count = OS_TICK_DURATION_ns * SYSTICK_CLOCK_MHz / CLOCK_SEC2MSEC;

        /* Setup SysTick clock source and enable interrupt */
        SYST_CVR = 0;
        SYST_RVR = tick_count & 0x00FFFFFF;
        SYST_CSR = SYSTICK_TICKINT + SYSTICK_ENABLE;
        return 0;
}

int brd_get_usec_syscount(u32 *ucount) {
        u32 count;

        *ucount = TIMELR; /* lower 32-bit of a 64-bit counter, incremented once per microsecond */

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

        return -1;

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
