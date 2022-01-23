#include <ostypes.h>
#include <stdarg.h>
#include <stdio.h>

#include <osek.h>
#include <sg_appmodes.h>
#include <sg_counter.h>

#include <os_api.h>


#include "qemu-versatilepb.h"


/* Serial console functions */
int console_fputc(const int c) {
        UART0DR = (unsigned int) (c);
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
/* System Clock = 1 MHz. The timer on our QEMU system defaults to using a 1MHz reference. */
#define SYSTEM_CLOCK_MHz        (1)

int brd_setup_systimer(void) {
        u32 tick_count = OS_TICK_DURATION_ns * SYSTEM_CLOCK_MHz / CLOCK_SEC2MSEC;

        /* Timer0 counter reload value init */
        *((volatile u32*)(TIMER0_BASE+TIMERLOAD_OFFSET)) = tick_count;

        /* Timer0 as system tick counter: Enable | Int. En | 32bit mode */
        *((volatile u8*)(TIMER0_BASE+TIMERCTRL_OFFSET)) = 0xE2;

        /* Timer1 as free running counter: Enable | 32bit mode */
        *((volatile u8*)(TIMER1_BASE+TIMERCTRL_OFFSET)) = 0x82;

        return 0;
}

int brd_get_usec_syscount(u32 *ucount) {
        u32 count;

        /* free running mode counts from 0xFFFFFFFF to 0, hence reversing it */
        count = (u32)0xFFFFFFFF - *((volatile u32*)(TIMER1_BASE+TIMERVALUE_OFFSET));

        /* convert to ucount so that the count value == 1 usec */
        *ucount = (u32)((count * 1000ull) / TIMER_CLK_TO_MILLISEC);
        //*ucount = count;

        return 0;
}


int brd_sys_enable_interrupts() {
        /* Enable interrupts */
        VIC_INTENABLE = 1 << ISR_SN_TIMER01;

        return 0;
}


int brd_console_init(void) {
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
