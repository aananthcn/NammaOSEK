#define PORT_ID_REG _u(0xD0000000) 

.syntax unified
.cpu cortex-m0plus
.thumb
.balign 2


.text

.global _Reset_handler

_Reset_handler:
	/* Never allow core 1 to execute this */
	ldr r0, =(PORT_ID_REG)
	ldr r0, [r0] /* loads the address of PORT_ID_REG */
	ldr r0, [r0] /* loads the value at &PORT_ID_REG */
	cmp r0, #0
	bne hold_non_core0_from_exec

	/* set program stack pointer */
	ldr r0, =_user_stack_top
	msr psp, r0
	/* set main stack pointer */
	ldr r0, =_os_stack_top
	msr msp, r0

	/* do .data initialization */
	ldr r0, =_init_data_rom
	ldr r1, =_data_start
	ldr r2, =_data_size
	movs r3, #0
copy_data:
	ldrb r4, [r0, r3]
	strb r4, [r1, r3]
	adds r3, r3, #1
	subs r2, r2, #1
	bne copy_data

	/* do .bss initialization */
	movs r0, #0
	ldr r1, =_bss_start
	ldr r2, =_bss_size
	movs r3, #0
copy_bss:
	strb r0, [r1, r3]
	adds r3, r3, #1
	subs r2, r2, #1
	bne copy_bss

	/* jump to main */
	bl main
	b .
 
hold_non_core0_from_exec:
    ldr r0, = 'W' | ('V' << 8)  /* _wait_for_vector */
    bl rom_data_lookup 		/* calling a non-returning function */
    bx r0

PORT_ID_REG:
.word 0xD0000000
 
.end
