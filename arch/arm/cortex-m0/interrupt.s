.syntax unified
.cpu cortex-m0plus
.thumb
.align 2

.text
 
.global __set_interrupt_bits
.global __clear_interrupt_bits


/*/
Interrupt Control State Register, ICSR
Note: ARMv6 doesn't support Co-Processor 
----------------------------------------------------------------------------------------------
Bits      | Name        | Function
----------------------------------------------------------------------------------------------
[31]      | NMIPENDSET  | 0 - do not activate NMI exception (also we can read back its status)
[30:29]   | Reserved    | 
[28]      | PENDSVSET   | 1 - Set pending PendSV interrupt. (also we can read back its status)
[27]      | PENDSVCLR   | 1 - Clear pending PendSV.
[26]      | PENDSTSET   | 1 - Set pending SysTick.
[25]      | PENDSTCLR   | 1 - Clear pending SysTick.
[24]      | Reserved    | 
[23]      | ISRPREEMPT  | 1 - Will service a pending exception. (Read Only)
[22]      | ISRPENDING  | 1 - Interrupt is pending. (Read Only)
[21]      | Reserved    | 
[20:12]   | VECTPENDING | 
[11:9]    | Reserved    | 
[8:0]     | VECTACTIVE  | 
----------------------------------------------------------------------------------------------

IRQ 	Interrupt Source
0 	TIMER_IRQ_0
1 	TIMER_IRQ_1
2 	TIMER_IRQ_2
3 	TIMER_IRQ_3
4 	PWM_IRQ_WRAP
5 	USBCTRL_IRQ
6 	XIP_IRQ
7 	PIO0_IRQ_0
8 	PIO0_IRQ_1
9 	PIO1_IRQ_0
10 	PIO1_IRQ_1
11 	DMA_IRQ_0
12 	DMA_IRQ_1
13 	IO_IRQ_BANK0
14 	IO_IRQ_QSPI
15 	SIO_IRQ_PROC0
16 	SIO_IRQ_PROC1
17 	CLOCKS_IRQ
18 	SPI0_IRQ
19 	SPI1_IRQ
20 	UART0_IRQ
21 	UART1_IRQ
22 	ADC0_IRQ_FIFO
23 	I2C0_IRQ
24 	I2C1_IRQ
25 	RTC_IRQ 

/*/


/*/
Function Name: _set_interrupt_bits
Arguments: arg1 (R0) = isr_bits, arg2 (R1) = addr
Returns: previous CPSR register value.
Description: This function sets the FIRQ and IRQ bits as passed as arguments.
             The users should noted that this function expects FIRQ and IRQ
             values to be passed at 6th and 7th bit position respectively.
/*/
 __set_interrupt_bits:
        ldr r2, [r1]
        orrs r2, r2, r0
        str r2, [r1] 
        mov pc, lr



/*/
Function Name: _clear_interrupt_bits
Arguments: arg1 (R0) = isr_bits, arg2 (R1) = addr
Returns: previous CPSR register value.
Description: This function clears the FIRQ and IRQ bits as passed as arguments.
             The users should noted that this function expects FIRQ and IRQ
             values to be passed at 6th and 7th bit position respectively.
/*/
 __clear_interrupt_bits:
        ldr r2, [r1]
        bics r2, r2, r0  @ r2 = r2 & (!r0)
        str r2, [r1] 
        mov pc, lr

.end
