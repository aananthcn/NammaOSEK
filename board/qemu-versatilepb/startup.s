.text
 .code 32
 
.global _reset_handler

_reset_handler:
	/* set Supervisor stack */
	ldr sp, =_os_stack_top
	/* copy vector table to address 0 */
	bl __copy_vectors
	/* get Program Status Register */
	mrs r0, cpsr
	/* go in IRQ mode */
	bic r1, r0, #0x1F
	orr r1, r1, #0x12
	msr cpsr, r1
	/* set IRQ stack */
	ldr sp, =_irq_stack_top
	/* Enable IRQs */
	bic r0, r0, #0x80
	/* go back in Supervisor mode */
	msr cpsr, r0

	/* do .data initialization */
	ldr r0, =_init_data_rom
	ldr r1, =_data_start
	ldr r2, =_data_size
copy_data:
	ldrb r4, [r0], #1
	strb r4, [r1], #1
	subs r2, r2, #1
	bne copy_data

	/* do .bss initialization */
	mov r0, #0
	ldr r1, =_bss_start
	ldr r2, =_bss_size
copy_bss:
	strb r0, [r1], #1
	subs r2, r2, #1
	bne copy_bss

	/* jump to main */
	bl main
	b .
 
.end
