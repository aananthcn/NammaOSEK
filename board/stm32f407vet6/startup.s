.syntax unified
.arch armv7e-m
.thumb
.balign 4

.text
 
.global _Reset_handler

_Reset_handler:
	/* set program stack pointer */
	ldr r0, =_user_stack_top
	msr psp, r0
	/* set main stack pointer */
	ldr r0, =_os_stack_top
	msr msp, r0
        /* Enter each mode in turn and set up the stack pointer */
        //MSR CPSR_c, #Mode_FIQ:OR:I_Bit:OR:F_Bit    ; Interrupts disabled
        //MOV sp, r0
        //add r0, r0, #_IRQ_STACK_SIZE
        //MSR CPSR_c, #Mode_IRQ:OR:I_Bit:OR:F_Bit    ; Interrupts disabled
        //MOV sp, r0
        # add R0, R0, #Len_IRQ_Stack
        //MSR CPSR_c, #Mode_SVC:OR:I_Bit:OR:F_Bit    ; Interrupts disabled
        //MOV sp, r0
        /* Leave processor in SVC mode */
	/* get Program Status Register */
	//mrs r0, cpsr
	/* go in IRQ mode */
	//bic r1, r0, #0x1F
	//orr r1, r1, #0x12
	//msr cpsr, r1
	/* set IRQ stack */
	//ldr sp, =_irq_stack_top
	/* Enable IRQs */
	//bic r0, r0, #0x80
	/* go back in Supervisor mode */
	//msr cpsr, r0

	/* do .data initialization */
	ldr r0, =_init_data_rom
	ldr r1, =_data_start
	ldr r2, =_data_size
	mov r3, #0
copy_data:
	ldrb r4, [r0, r3]
	strb r4, [r1, r3]
	add r3, r3, #1
	subs r2, r2, #1
	bne copy_data

	/* do .bss initialization */
	mov r0, #0
	ldr r1, =_bss_start
	ldr r2, =_bss_size
	mov r3, #0
copy_bss:
	strb r0, [r1, r3]
	add r3, r3, #1
	subs r2, r2, #1
	bne copy_bss

	/* jump to main */
	bl main
	b .
 
.end
