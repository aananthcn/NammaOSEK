.text
 .code 32
 
.global reset_handler

reset_handler:
	/* set Supervisor stack */
	LDR sp, =_os_stack_top
	/* copy vector table to address 0 */
	BL __copy_vectors
	/* get Program Status Register */
	MRS r0, cpsr
	/* go in IRQ mode */
	BIC r1, r0, #0x1F
	ORR r1, r1, #0x12
	MSR cpsr, r1
	/* set IRQ stack */
	LDR sp, =_irq_stack_top
	/* Enable IRQs */
	BIC r0, r0, #0x80
	/* go back in Supervisor mode */
	MSR cpsr, r0
	/* jump to main */
	BL main
	B .
 
.end
