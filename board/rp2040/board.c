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
#define XOSC_MHz                (12)
#define MHz                     (1000000)

int brd_osc_init(void) {
        XOSC_CTRL    = XOSC_FREQ_RANGE_1_15MHz;
        XOSC_STARTUP = XOSC_DELAY((((XOSC_MHz * MHz) / CLOCK_SEC2MSEC) + 128) / 256);
        XOSC_CTRL   |= XOSC_ENABLE;
        /* wait till XOSC becomes stable */
        while (!(XOSC_STATUS & 0x80000000));
}



int brd_config_clock(u32 reg_base, u8 clksrc, u8 auxsrc) {
        /* First reset the Clock src, else system may hang */
        *(volatile u32*)(reg_base + CLK_CTRL) &= ~(0x3);
        while(!((*(volatile u32*)(reg_base + CLK_SELECTED)) & (1)));

        /* Clock DIV register */
        *(volatile u32*)(reg_base + CLK_DIV) = (1 << 8);
        /* Clock CTRL register */
        *(volatile u32*)(reg_base + CLK_CTRL) = (auxsrc << 5) | (clksrc);
        /* Wait till the clock source bit in Clock SELECTED register is set */
        while(!((*(volatile u32*)(reg_base + CLK_SELECTED)) & (1 << clksrc)));

        return 0;
}


int brd_clock_init(void) {
        u32 clksrc, auxsrc;

        /* CLK_REF SRC = XOSC = 12 MHz, No AUXSRC */
        brd_config_clock(CLK_REF_BASE, 2, 0);

        /* CLK_SYS SRC = SYS_AUX, AUXSRC = PLL_SYS = 125 MHz */
        brd_config_clock(CLK_SYS_BASE, 1, 0);

        return 0;
}



/*/               REFDIV   XOSC    REF   VCO       POSTDIV   SYS_PLL
    SYS PLL: 12 / 1    =   12MHz * 125 = 1500MHz / 6 / 2   = 125MHz
/*/
int brd_sys_pll_init(void) {
        u32 vco_freq = (1500 * MHz); /* min = 5 MHz, max = 1600 MHz */
        u32 refdiv = 1;
        u32 fbdiv, post_div1, post_div2, ref_mhz;

        /* let us switch to ROSC clock before PLL power down */
        brd_config_clock(CLK_SYS_BASE, 0, 2);
        /* power down PLL to configure it correctly */
        PLL_PWR = (1 << 5) | (1 << 3) | (1 << 2) | (1 << 0);

        /* do frequency checks */
        fbdiv = vco_freq / (XOSC_MHz * MHz);
        if ((fbdiv < 16) || (fbdiv >320)) {
                pr_log("Error: fbdiv out of range!\n");
                return -1;
        }
        post_div1 = 6; /* min = 1, max = 7 */
        post_div2 = 2; /* min = 1, max = 7 */
        if (post_div2 > post_div1) {
                pr_log("Error: post_div2 greater than post_div1\n");
                return -1;
        }
        ref_mhz = XOSC_MHz / refdiv;
        if (ref_mhz > (vco_freq / 16)) {
                pr_log("Error: reference freq greater than vco / 16\n");
                return -1;
        }

        /* configure the SYS PLL */
        PLL_FBDIV_INT = fbdiv;
        PLL_PWR &= ~((1 << 5) | (1 << 0)); /* clear VCOPD and PD bits */

        /* wait till PLL gets locked */
        while (!(PLL_CS & 0x80000000));

        /* configure post dividers */
        PLL_PRIM = post_div1 << 16 | post_div2 << 12;
        PLL_PWR &= ~(1 << 3); /* clear POSTDIVPD bit */
        return 0;
}


int brd_setup_systimer(void) {
        register u32 tick_count = OS_TICK_DURATION_ns * XOSC_MHz / CLOCK_SEC2MSEC;

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
        /* let us drive UART (peri_clk) from 12 MHz crystal oscillator */
        CLK_PERI_CTRL = PERI_ENABLE | PERI_AUXSRC(0x4);
        return -1;

        pr_log_init();

        return 0;
}


void main(void) {
        brd_osc_init();
        brd_sys_pll_init();
        brd_clock_init();
        brd_setup_systimer();
        brd_console_init();
        brd_sys_enable_interrupts();

        StartOS(OSDEFAULTAPPMODE);
        /* The execution should never reach here */
        pr_log("Info: StartOS() function returned!! OS Exits!\n");
}
