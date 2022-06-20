#ifndef VERSATILEPB_H
#define VERSATILEPB_H

/* Register definitions for versatilepb board */

#include <ostypes.h>
#include <stm32f407vet6/board.h>


/* Interrupt Controller */
#define VIC_BASE_ADDR   0x10140000
#define VIC_IRQSTATUS   (*((volatile u32 *)(VIC_BASE_ADDR + 0x000)))
#define VIC_INTENABLE   (*((volatile u32 *)(VIC_BASE_ADDR + 0x010)))

/* ISR shift numbers */
#define ISR_SN_TIMER01  (4)
#define ISR_SN_TIMER23  (5)
#define ISR_SN_UART0    (12)



/* Clock & Oscillator Registers */
//================================
/* RCC clock control register */
#define RCC_CR                  (*((volatile u32 *)(0x40023800)))
#define RCC_PLLCFGR             (*((volatile u32 *)(0x40023804)))
#define RCC_CFGR                (*((volatile u32 *)(0x40023808)))
#define RCC_APB1RSTR            (*((volatile u32 *)(0x40023820)))
#define RCC_AHB1ENR             (*((volatile u32 *)(0x40023830)))
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



/* STM32F407VE Port A Registers */
//================================
#define GPIOA_MODER             (*((volatile u32 *)(0x40020000)))
#define GPIOA_ODR		(*((volatile u32 *)(0x40020014)))
#define GPIOx_BSRR		(*((volatile u32 *)(0x40020018)))
#define GPIOA_AFRL              (*((volatile u32 *)(0x40020020)))
#define GPIOA_AFRH              (*((volatile u32 *)(0x40020024)))



/* UART / USART Registers */
//================================
#define USART1_SR         	(*((volatile u32 *)(0x40011000)))
#define USART1_DR         	(*((volatile u32 *)(0x40011004)))
#define USART1_BRR        	(*((volatile u32 *)(0x40011008)))
#define USART1_CR1        	(*((volatile u32 *)(0x4001100C)))
#define USART1_CR2        	(*((volatile u32 *)(0x40011010)))
#define USART1_CR3        	(*((volatile u32 *)(0x40011014)))
#define USART1_GTPR       	(*((volatile u32 *)(0x40011018)))



#endif