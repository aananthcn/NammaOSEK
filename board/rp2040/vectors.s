.syntax unified
.cpu cortex-m0plus
.thumb
.align 2
 

.section .vectors, "ax"
 /* .code 32 */

.global _vectors_start
.global _vectors_end
_vectors_start:
        .word	_os_stack_top
        .word	(_Reset_handler)        /* Exception no  1 */
        .word	_NMI_handler 	        /* Exception no  2 */
        .word	_HardFault_handler      /* Exception no  3 */
        .word	0                       /* Reserved   -  4 */
        .word	0                       /* Reserved   -  5 */
        .word	0                       /* Reserved   -  6 */
        .word	0                       /* Reserved   -  7 */
        .word	0                       /* Reserved   -  8 */
        .word	0                       /* Reserved   -  9 */
        .word	0                       /* Reserved   - 10 */
        .word	_SVCall_handler         /* Exception no 11 */
        .word	0                       /* Reserved   - 12 */
        .word	0                       /* Reserved   - 13 */
        .word	_PendSV_handler         /* Exception no 14 */
        .word	_SysTick_handler        /* Exception no 15 */
        .word	_irq_handler            /* Exception no 16 */ 
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        .word 	0
        
_vectors_end:
        B .

