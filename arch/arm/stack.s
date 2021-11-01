.text
 .code 32
 
.global _switch_stack

_switch_stack:
	/* note down the place where this call came from */
	mov r2, lr

	/* store the current stack pointer in r1 */
	mov r1, sp

	/* TODO: Disable IRQ and FIQ */
	bl _disable_interrupts

	/* r0 - arg1, r1 - arg2 */
	mov sp, r0

	/* TODO: Enable IRQ and FIQ */
	bl _enable_interrupts

	/* pass old stack as return value */
	mov r0, r1

	/* go back to the place where this was called from (os_task.c) */
	mov pc, r2

.end
