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



/* Clock & Oscillator Registers */
/*/================================
RP2040 External Clock: 12 MHz
RP2040 System Clock:  125 MHz (max)
================================/*/
#define SYST_CSR_OFFSET         (0xe010)
#define SYSTICK_TICKINT         (0x02)
#define SYSTICK_ENABLE          (0x01)
#define SYST_CSR                (*((volatile u32 *)(PPB_BASE + SYST_CSR_OFFSET)))

#define SYST_RVR_OFFSET         (0xe014)
#define SYST_RVR                (*((volatile u32 *)(PPB_BASE + SYST_RVR_OFFSET)))
#define SYST_CVR_OFFSET         (0xe018)
#define SYST_CVR                (*((volatile u32 *)(PPB_BASE + SYST_CVR_OFFSET)))



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
