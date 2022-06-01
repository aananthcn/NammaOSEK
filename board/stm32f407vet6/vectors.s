.syntax unified
.arch armv7e-m
.thumb
.balign 4
 

.text
 /* .code 32 */

_vectors_start:
        .long	_os_stack_top
        .long	(_Reset_handler+1)      /* Exception no 1, +1 is for setting the T-bit in EPSR */
        .long	_NMI_handler 	        /* Exception no 2 */
        .long	_HardFault_handler_asm  /* Exception no 3 */
        .long	_MemManage_handler      /* Exception no 4 */
        .long	_BusFault_handler       /* Exception no 5 */
        .long	_UsageFault_handler     /* Exception no 6 */
        .long	0                       /* Reserved  -  7 */
        .long	0                       /* Reserved  -  8 */
        .long	0                       /* Reserved  -  9 */
        .long	0                       /* Reserved  - 10 */
        .long	_SVCall_handler         /* Exception no 11 */
        .long	_DebugMonitor_handler   /* Exception no 12 */
        .long	0                       /* Reserved - 13 */
        .long	_PendSV_handler         /* Exception no 14 */
        .long	_SysTick_handler        /* Exception no 15 */
        .long	_irq_handler            /* Exception no 16 */ 
        
_vectors_end:
        B .


_HardFault_handler_asm:
        tst    LR, #4            ;// Check EXC_RETURN in Link register bit 2.
        ite    EQ
        mrseq  R0, MSP           ;// Stacking was using MSP.
        mrsne  R0, PSP           ;// Stacking was using PSP.        
        b      _HardFault_handler
