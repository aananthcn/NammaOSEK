#ifndef VERSATILEPB_H
#define VERSATILEPB_H

/* Register definitions for versatilepb board */

#include <ostypes.h>

/* Interrupt Controller */
#define VIC_BASE_ADDR   0x10140000
#define VIC_IRQSTATUS 	(*((volatile u32 *)(VIC_BASE_ADDR + 0x000)))
#define VIC_INTENABLE 	(*((volatile u32 *)(VIC_BASE_ADDR + 0x010)))

/* ISR shift numbers */
#define ISR_SN_TIMER01  (4)
#define ISR_SN_TIMER23  (5)
#define ISR_SN_UART0    (12)

/* UART */
#define UART0DR 	(*((volatile u32 *)0x101f1000))

/* Timer blocks */
#define TIMER0_BASE 	(0x101E2000)
#define TIMER1_BASE 	(0x101E2020)
#define TIMER2_BASE 	(0x101E3000)
#define TIMER3_BASE 	(0x101E3020)
/* Timer register offsets */
#define TIMERLOAD_OFFSET        (0x00)
#define TIMERVALUE_OFFSET       (0x04)
#define TIMERCTRL_OFFSET        (0x08)
#define TIMERINTCLR_OFFSET      (0x0C)
#define TIMERINTSTS_OFFSET      (0x10)
#define TIMERMSKINTSTS_OFFSET   (0x14)
#define TIMERBGLOAD_OFFSET      (0x18)


#endif