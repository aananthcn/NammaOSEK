.text
 .code 32
 
.global _disable_interrupts
.global _enable_interrupts


_disable_interrupts:
	push {r0, lr}
	mrs r0, cpsr
	orr r0, r0, #0xC0
	msr cpsr, r0
	pop {r0, pc}


_enable_interrupts:
	push {r0, lr}
	mrs r0, cpsr
	bic r0, r0, #0xC0
	msr cpsr, r0
	pop {r0, pc}
 
.end
