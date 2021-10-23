#include <ostypes.h>
#include <stdarg.h>
#include <stdio.h>

#include <osek.h>
#include <sg_appmodes.h>
#include <os_api.h>


#include "versatilepb.h"


int console_fputs(const char *s) {
        int count = 0;

        while (*s != '\0') {
                UART0DR = (unsigned int)(*s);
                count++;
                s++;
        }

        return count;
}

int console_fputc(const int c) {
        UART0DR = (unsigned int) (c);
        return c;
}


int brd_setup_sytimer(void) {
        /* Timer0 counter reload value init */
        *((volatile u32*)(TIMER0_BASE+TIMERLOAD_OFFSET)) = 1000;

        /* Timer0 as system tick counter: Enable | 32bit mode */
        *((volatile u8*)(TIMER0_BASE+TIMERCTRL_OFFSET)) = 0x82;

        /* Timer1 as free running counter: Enable | 32bit mode */
        *((volatile u8*)(TIMER1_BASE+TIMERCTRL_OFFSET)) = 0x82;

        return 0;
}

int brd_get_usec_syscount(u32 *ucount) {
        /* free running mode counts from 0xFFFFFFFF to 0, hence reversing it */
        *ucount = (u32)0xFFFFFFFF - *((volatile u32*)(TIMER1_BASE+TIMERVALUE_OFFSET));
        return 0;
}


int brd_sys_enable_interrupts() {
        /* Enable interrupts */
        VIC_INTENABLE = 1 << ISR_SN_TIMER01;

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
