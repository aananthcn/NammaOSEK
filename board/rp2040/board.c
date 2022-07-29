#include <ostypes.h>
#include <stdarg.h>
#include <stdio.h>

#include <osek.h>
#include <sg_appmodes.h>
#include <sg_counter.h>

#include <os_api.h>


#include "rp2040.h"



/* Macros */
#define CLOCK_SEC2MSEC          (1000) /* 1000ms = 1 sec */

/* SysTick clk = External Clock = 12 MHz; let us not use 125MHz System clock for SysTick */
#define XOSC_MHz                (12)
#define CPU_CLK_MHz             (125)
#define PERI_CLK_MHz            (125)
#define MHz                     (1000000)


/* Functions */
int uc_uart_init(u32 base) {
        u32 baud_divint, baud_divfrac;
        u32 baud_rate_div;

        /* set PAD, FUNCTSEL for GPIO 0 (TX0) and 1 (RX0) pins */
        SET_PAD_GPIO(0, (1<<7)|(1<<6));
        SET_GPIO_CTRL(0, GPIO_FUNC_UART);
        SET_PAD_GPIO(1, (1<<7)|(1<<6));
        SET_GPIO_CTRL(1, GPIO_FUNC_UART);

        /* FreeOSEK's standard baudrate = 115200, CLK_PERI = 125 MHz */
        baud_rate_div = ((PERI_CLK_MHz * MHz) << 3) / (115200);
        baud_divint = baud_rate_div >> (3+4);
        if (baud_divint == 0) {
                baud_divint = 1;
                baud_divfrac = 0;
        }
        else if (baud_divint >= 65535) {
                baud_divint = 65535;
                baud_divfrac = 0;
        }
        else {
                /* extract 6 bits from baud_divfrac */
                baud_divfrac = ((baud_rate_div & 0x7f) + 1) / 2;
        }

        /* Set computed baudrate values to registers */
        UART0_IBRD = baud_divint; //16 bits
        UART0_FBRD = baud_divfrac; //6 bits

        /* UART Data Format & configs */
        UART0_LCR_H = (0x3 << 5) /* 8-bit */ | (1 << 4) /* FEN */;

        /* Enable Tx, Rx and UART0 */
        UART0_CR = (1 << 9) | (1 << 8) | (1 << 0);

        return 0;
}


/* Serial console functions */
int console_fputc(const int c) {
        u32 x;
        x = UART0_DR;
        UART0_DR = (unsigned int) (c & 0xFF);
        return x;
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


/* Microcontroller Subsystem Reset */
int uc_ss_reset(void) {
        RESET_CTRL = ~(
                SS_BIT_TIMER |
                SS_BIT_PLL_SYS |
                SS_BIT_PLL_USB |
                SS_BIT_PADS_QSPI |
                SS_BIT_IO_QSPI |
                SS_BIT_PADS_BANK0 |
                SS_BIT_IO_BANK0 |
                SS_BIT_JTAG |
                SS_BIT_UART0
        );
        return 0;
}



int uc_osc_init(void) {
        XOSC_CTRL    = XOSC_FREQ_RANGE_1_15MHz;
        XOSC_STARTUP = XOSC_DELAY((((XOSC_MHz * MHz) / CLOCK_SEC2MSEC) + 128) / 256);
        XOSC_CTRL   |= XOSC_ENABLE;
        /* wait till XOSC becomes stable */
        while (!(XOSC_STATUS & 0x80000000));
}



int uc_config_clock(u32 reg_base, u8 clksrc, u8 auxsrc) {
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


int uc_clock_init(void) {
        u32 clksrc, auxsrc;

        /* CLK_REF: SRC = XOSC = 12 MHz, No AUXSRC */
        uc_config_clock(CLK_REF_BASE, 2, 0);

        /* CLK_SYS: SRC = SYS_AUX, AUXSRC = PLL_SYS = 125 MHz */
        uc_config_clock(CLK_SYS_BASE, 1, 0);

        /* CLK_PERI: AUXSRC = CLK_SYS = 125 MHz */
        CLK_PERI_CTRL = PERI_ENABLE | PERI_AUXSRC(0);

        /* CLK_USB: SRC = none, AUXSRC = PSS_USB = 48 MHz */

        /* CLK_GPOUTx: use 12 MHz crystal clock as clock source */
        /* Not sure if this initialization is needed for normal GPIO usage */
        CLK_GPOUT0_CTRL = 1 << 11 | 6 << 5;
        CLK_GPOUT0_DIV = 10 << 8;
        CLK_GPOUT1_CTRL = 10 << 11 | 6 << 5;
        CLK_GPOUT1_DIV = 10 << 8;
        CLK_GPOUT2_CTRL = 10 << 11 | 6 << 5;
        CLK_GPOUT2_DIV = 10 << 8;
        CLK_GPOUT3_CTRL = 10 << 11 | 6 << 5;
        CLK_GPOUT3_DIV = 10 << 8;

        return 0;
}



/*/               REFDIV   XOSC    REF   VCO       POSTDIV   SYS_PLL
    SYS PLL: 12 / 1    =   12MHz * 125 = 1500 MHz / 6 / 2  = 125 MHz
    USB PLL: 12 / 1    =   12MHz * 40  =  480 MHz / 5 / 2  =  48 MHz
/*/
int uc_pll_init(u32 base, u32 vco_freq_mhz, u8 post_div1, u8 post_div2) {
        u32 vco_freq = (vco_freq_mhz * MHz); 
        u32 refdiv = 1;
        u32 fbdiv, ref_mhz;

        /* let us switch to ROSC clock before PLL power down */
        uc_config_clock(CLK_SYS_BASE, 0, 2);
        uc_config_clock(CLK_REF_BASE, 0, 0);

        /* power down PLL to configure it correctly */
        *((volatile u32*)(base+PLL_PWR_OFFSET)) = (1 << 5) | (1 << 3) | (1 << 2) | (1 << 0);

        /* do frequency checks */
        fbdiv = vco_freq / (XOSC_MHz * MHz);
        if ((fbdiv < 16) || (fbdiv >320)) {
                pr_log("Error: fbdiv out of range!\n");
                return -1;
        }
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
        *((volatile u32*)(base+PLL_FBDIV_INT_OFFSET)) = fbdiv;
        *((volatile u32*)(base+PLL_PWR_OFFSET)) &= ~((1 << 5) | (1 << 0)); /* clear VCOPD and PD bits */

        /* wait till PLL gets locked */
        while (!(*((volatile u32*)(base+PLL_CS_OFFSET)) & 0x80000000));

        /* configure post dividers */
        *((volatile u32*)(base+PLL_PRIM_OFFSET)) = post_div1 << 16 | post_div2 << 12;
        *((volatile u32*)(base+PLL_PWR_OFFSET)) &= ~(1 << 3); /* clear POSTDIVPD bit */
        return 0;
}


int brd_setup_systimer(void) {
        register u32 tick_count = OS_TICK_DURATION_ns * CPU_CLK_MHz / CLOCK_SEC2MSEC;

        /* Setup SysTick clock source and enable interrupt */
        SYST_CVR = 0;
        SYST_RVR = tick_count & 0x00FFFFFF;
        SYST_CSR = (1 << 2) | (1 << 1) | (1 << 0); /* CLK SRC = Proc. Clk; TICKINT = ISR Trig.; ENABLE = 1 */
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
        uc_uart_init(UART0_BASE);
        pr_log_init();

        return 0;
}


void main(void) {
        uc_ss_reset();
        uc_osc_init();
        uc_pll_init(PLL_SYS_BASE, 1500, 6, 2); /*SYS_VCO: min = 5 MHz, max = 1600 MHz */
        uc_pll_init(PLL_USB_BASE, 480, 5, 2);
        uc_clock_init();
        brd_setup_systimer();
        brd_console_init();
        brd_sys_enable_interrupts();

        StartOS(OSDEFAULTAPPMODE);
        /* The execution should never reach here */
        pr_log("Info: StartOS() function returned!! OS Exits!\n");
}
