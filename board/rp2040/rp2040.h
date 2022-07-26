#ifndef VERSATILEPB_H
#define VERSATILEPB_H

/* Register definitions for versatilepb board */

#include <ostypes.h>


/* Interrupt Macros */
#define DISABLE_INTERRUPT       (0x00000000)
#define ENABLE_INTERRUPT        (0x00000000)


/* Interrupt Controller in RP2040 */
//==================================
#define SIO_BASE                (0xd0000000)
#define PPB_BASE                (0xe0000000)


/* Interrupt Control and State Register */
#define ICSR_OFFSET             0xed04
#define ICSR                    *((volatile u32 *)(PPB_BASE + ICSR_OFFSET)))
/* Interrupt Set-Enable Register */
#define NVIC_ISER_OFFSET        0xe100
#define NVIC_ISER               (*((volatile u32 *)(PPB_BASE + NVIC_ISER_OFFSET)))
/* Interrupt Clear-Enable Register */
#define NVIC_ICER_OFFSET        0xe180
#define NVIC_ICER               (*((volatile u32 *)(PPB_BASE + NVIC_ICER_OFFSET)))


/* ISR shift numbers */
#define ISR_SN_TIMER01          (4)
#define ISR_SN_TIMER23          (5)
#define ISR_SN_UART0            (12)


/* SysTick Registers */
#define SYST_CSR_OFFSET         (0xe010)
#define SYSTICK_TICKINT         (0x02)
#define SYSTICK_ENABLE          (0x01)
#define SYST_CSR                (*((volatile u32 *)(PPB_BASE + SYST_CSR_OFFSET)))

#define SYST_RVR_OFFSET         (0xe014)
#define SYST_RVR                (*((volatile u32 *)(PPB_BASE + SYST_RVR_OFFSET)))
#define SYST_CVR_OFFSET         (0xe018)
#define SYST_CVR                (*((volatile u32 *)(PPB_BASE + SYST_CVR_OFFSET)))


/* Clock & Oscillator Registers */
/*/================================
RP2040 External Clock: 12 MHz
RP2040 System Clock:  125 MHz (max)
================================/*/
#define XOSC_BASE               (0x40024000)

#define XOSC_CTRL_OFFSET        (0x00)
#define XOSC_CTRL               (*((volatile u32 *)(XOSC_BASE + XOSC_CTRL_OFFSET)))
#define XOSC_FREQ_RANGE_1_15MHz (0xaa0)
#define XOSC_ENABLE             (0xfab << 12)
#define XOSC_DISABLE            (0xd1e << 12)

#define XOSC_STATUS_OFFSET      (0x04)
#define XOSC_STATUS             (*((volatile u32 *)(XOSC_BASE + XOSC_STATUS_OFFSET)))

#define XOSC_STARTUP_OFFSET     (0x0C)
#define XOSC_STARTUP            (*((volatile u32 *)(XOSC_BASE + XOSC_STATUS_OFFSET)))
#define XOSC_DELAY(x)           (x << 0)


/* SYS PLL */
#define PLL_SYS_BASE            (0x40028000)
#define PLL_CS_OFFSET           (0x0)
#define PLL_PWR_OFFSET          (0x4)
#define PLL_FBDIV_INT_OFFSET    (0x8)
#define PLL_PRIM_OFFSET         (0xc)
#define PLL_CS                  (*((volatile u32 *)(PLL_SYS_BASE + PLL_CS_OFFSET)))
#define PLL_PWR                 (*((volatile u32 *)(PLL_SYS_BASE + PLL_PWR_OFFSET)))
#define PLL_FBDIV_INT           (*((volatile u32 *)(PLL_SYS_BASE + PLL_FBDIV_INT_OFFSET)))
#define PLL_PRIM                (*((volatile u32 *)(PLL_SYS_BASE + PLL_PRIM_OFFSET)))


/* Clocks */
#define CLOCKS_BASE             (0x40008000)
#define CLK_CTRL                (0x0)
#define CLK_DIV                 (0x4)
#define CLK_SELECTED            (0x8)

#define CLK_REF_BASE            (CLOCKS_BASE + 0x30)
#define CLK_REF_CTRL            (*((volatile u32 *)(CLK_REF_BASE + CLK_CTRL)))
#define CLK_REF_DIV             (*((volatile u32 *)(CLK_REF_BASE + CLK_DIV)))
#define CLK_REF_SELECTED        (*((volatile u32 *)(CLK_REF_BASE + CLK_SELECTED)))

#define CLK_SYS_BASE            (CLOCKS_BASE + 0x3c)
#define CLK_SYS_CTRL            (*((volatile u32 *)(CLK_SYS_BASE + CLK_CTRL)))
#define CLK_SYS_DIV             (*((volatile u32 *)(CLK_SYS_BASE + CLK_DIV)))
#define CLK_SYS_SELECTED        (*((volatile u32 *)(CLK_SYS_BASE + CLK_SELECTED)))

#define CLK_PERI_CTRL           (*((volatile u32 *)(CLOCKS_BASE + 0x48)))
#define PERI_AUXSRC(x)          (x << 5)
#define PERI_ENABLE             (1 << 11)


/* Raspberry Pi Pico (RP2040) Timer Register */
//============================================
#define TIMER_BASE              (0x40054000)
#define TIMEHW_OFFSET           (0x00)
#define TIMELW_OFFSET           (0x04)
#define TIMEHR_OFFSET           (0x08)
#define TIMELR_OFFSET           (0x0C)

#define TIMELR                  (*((volatile u32 *)(TIMER_BASE + TIMELR_OFFSET)))


/* STM32F407VE Port A Registers */
//================================
#define GPIOA_MODER             (*((volatile u32 *)(0x40020000)))
#define GPIOA_ODR               (*((volatile u32 *)(0x40020014)))
#define GPIOx_BSRR              (*((volatile u32 *)(0x40020018)))
#define GPIOA_AFRL              (*((volatile u32 *)(0x40020020)))
#define GPIOA_AFRH              (*((volatile u32 *)(0x40020024)))



/* UART / USART Registers */
//================================
#define USART1_SR               (*((volatile u32 *)(0x40011000)))
#define USART1_DR               (*((volatile u32 *)(0x40011004)))
#define USART1_BRR              (*((volatile u32 *)(0x40011008)))
#define USART1_CR1              (*((volatile u32 *)(0x4001100C)))
#define USART1_CR2              (*((volatile u32 *)(0x40011010)))
#define USART1_CR3              (*((volatile u32 *)(0x40011014)))
#define USART1_GTPR             (*((volatile u32 *)(0x40011018)))



#endif
