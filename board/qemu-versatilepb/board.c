#include <ostypes.h>
#include <stdarg.h>
#include <stdio.h>

#include <osek.h>
#include <sg_appmodes.h>
#include <os_api.h>


/* Interrupt Controller */
#define VIC_BASE_ADDR   0x10140000
#define VIC_INTENABLE (*((volatile u32 *)(VIC_BASE_ADDR + 0x010)))

/* ISR shift numbers */
#define ISR_SN_TIMER01  (4)
#define ISR_SN_TIMER23  (5)
#define ISR_SN_UART0    (12)

/* UART */
volatile unsigned int *const UART0DR = (unsigned int *)0x101f1000;

/* Timer */
volatile unsigned char *const TIMER0_BASE = (unsigned char *)0x101E2000;
volatile unsigned char *const TIMER1_BASE = (unsigned char *)0x101E2020;
volatile unsigned char *const TIMER2_BASE = (unsigned char *)0x101E3000;
volatile unsigned char *const TIMER3_BASE = (unsigned char *)0x101E3020;
#define TIMERLOAD_OFFSET        (0x00)
#define TIMERVALUE_OFFSET       (0x04)
#define TIMERCTRL_OFFSET        (0x08)
#define TIMERINTCLR_OFFSET      (0x0C)
#define TIMERINTSTS_OFFSET      (0x10)
#define TIMERMSKINTSTS_OFFSET   (0x14)
#define TIMERBGLOAD_OFFSET      (0x18)



int console_fputs(const char *s) {
        int count = 0;

        while (*s != '\0') {
                *UART0DR = (unsigned int)(*s);
                count++;
                s++;
        }

        return count;
}

int console_fputc(const int c) {
        *UART0DR = (unsigned int) (c);
        return c;
}


int brd_setup_sytimer(void) {
        /* Enable Timer0, free running, 32bit mode */
        *((u8*)(TIMER1_BASE+TIMERCTRL_OFFSET)) = 0x82;

        return 0;
}

int brd_get_usec_syscount(u32 *ucount) {
        /* free running mode counts from 0xFFFFFFFF to 0, hence reversing it */
        *ucount = (u32)0xFFFFFFFF - *((u32*)(TIMER1_BASE+TIMERVALUE_OFFSET));
        return 0;
}


int brd_sys_enable_interrupts() {
        /* Enable interrupts */
        VIC_INTENABLE = 1 << ISR_SN_TIMER01;
        
        return 0;
}


void main(void) {
        static bool hello_print = false;

        if (hello_print == false) {
                pr_log_init();
                brd_setup_sytimer();
                brd_sys_enable_interrupts();

                StartOS(OSDEFAULTAPPMODE);
                /* The execution should never reach here */
                pr_log("Info: StartOS() function returned!! OS Exits!\n");
                hello_print = true;
        }
}
