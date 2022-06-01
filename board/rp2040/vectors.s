.syntax unified
.cpu cortex-m0plus
.thumb
.balign 2
 

.text
 /* .code 32 */

_vectors_start:
        .long	_os_stack_top
        .long	(_Reset_handler+1)      /* Exception no  1, +1 is for setting the T-bit in EPSR */
        .long	_NMI_handler 	        /* Exception no  2 */
        .long	_HardFault_handler      /* Exception no  3 */
        .long	0                       /* Reserved   -  4 */
        .long	0                       /* Reserved   -  5 */
        .long	0                       /* Reserved   -  6 */
        .long	0                       /* Reserved   -  7 */
        .long	0                       /* Reserved   -  8 */
        .long	0                       /* Reserved   -  9 */
        .long	0                       /* Reserved   - 10 */
        .long	_SVCall_handler         /* Exception no 11 */
        .long	0                       /* Reserved   - 12 */
        .long	0                       /* Reserved   - 13 */
        .long	_PendSV_handler         /* Exception no 14 */
        .long	_SysTick_handler        /* Exception no 15 */
        .long	_irq_handler            /* Exception no 16 */ 
        
_vectors_end:
        B .

