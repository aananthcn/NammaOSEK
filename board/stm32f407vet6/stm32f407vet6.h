#ifndef VERSATILEPB_H
#define VERSATILEPB_H

/* Register definitions for versatilepb board */

#include <ostypes.h>

/* Interrupt Controller */
#define VIC_BASE_ADDR   0x10140000
#define VIC_IRQSTATUS   (*((volatile u32 *)(VIC_BASE_ADDR + 0x000)))
#define VIC_INTENABLE   (*((volatile u32 *)(VIC_BASE_ADDR + 0x010)))

/* ISR shift numbers */
#define ISR_SN_TIMER01  (4)
#define ISR_SN_TIMER23  (5)
#define ISR_SN_UART0    (12)

/* UART */
#define UART0DR         (*((volatile u32 *)0x101f1000))


/* Clock & Oscillator Registers */
//================================
/* RCC clock control register */
#define RCC_CR                  (*((volatile u32 *)(0x40023800)))
#define RCC_PLLCFGR             (*((volatile u32 *)(0x40023804)))
#define RCC_CFGR                (*((volatile u32 *)(0x40023808)))
#define RCC_APB1RSTR            (*((volatile u32 *)(0x40023820)))
#define RCC_APB1ENR             (*((volatile u32 *)(0x40023840)))
#define RCC_APB2ENR             (*((volatile u32 *)(0x40023844)))


/* Timer Registers */
//================================
/* SysTick Control and Status Register */
#define CM4_SYSTICK_STCSR       (*((volatile u32 *)(0xE000E010))) 
/* SysTick Reload Value Register */
#define CM4_SYSTICK_STRVR       (*((volatile u32 *)(0xE000E014)))
/* SysTick Current Value Register */
#define CM4_SYSTICK_STCVR       (*((volatile u32 *)(0xE000E018)))
/* SysTick Calibration Value Register */
#define CM4_SYSTICK_STCR        (*((volatile u32 *)(0xE000E01C)))

/* STM32F407ve Timer2 Register */
//================================
/* Control Registers */
#define TIM2_CR1                (*((volatile u32 *)(0x40000000)))
#define TIM2_CR2                (*((volatile u32 *)(0x40000004)))
/* Slave Mode Control Register */
#define TIM2_SMCR               (*((volatile u32 *)(0x40000008)))
/* DMA/Interrupt Enable Register */
#define TIM2_DIER               (*((volatile u32 *)(0x4000000C)))
/* Status Register */
#define TIM2_SR                 (*((volatile u32 *)(0x40000010)))
/* Event Generation Register */
#define TIM2_EGR                (*((volatile u32 *)(0x40000014)))
/* Counter Register */
#define TIM2_CNT                (*((volatile u32 *)(0x40000024)))
/* Prescalar Register */
#define TIM2_PSC                (*((volatile u32 *)(0x40000028)))


#endif