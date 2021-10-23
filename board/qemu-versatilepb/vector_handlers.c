#include <ostypes.h>
#include <stdarg.h>
#include <stdio.h>


void __attribute__((interrupt)) irq_handler() {
	printf("Entered function: %s()\n", __func__);
}

void __attribute__((interrupt)) undef_handler(void) {
	printf("Entered a trap function: %s()\n", __func__);
	for (;;);
}

void __attribute__((interrupt)) swi_handler(void) {
	printf("Entered a trap function: %s()\n", __func__);
	for (;;);
}

void __attribute__((interrupt)) prefetch_abort_handler(void) {
	printf("Entered a trap function: %s()\n", __func__);
	for (;;);
}

void __attribute__((interrupt)) data_abort_handler(void) {
	printf("Entered a trap function: %s()\n", __func__);
	for (;;);
}

void __attribute__((interrupt)) fiq_handler(void) {
	printf("Entered a trap function: %s()\n", __func__);
	for (;;);
}

void __copy_vectors(void) {
	extern uint32_t vectors_start;
	extern uint32_t vectors_end;
	uint32_t *vectors_src = &vectors_start;
	uint32_t *vectors_dst = (uint32_t *)0;

	while (vectors_src < &vectors_end)
		*vectors_dst++ = *vectors_src++;
}