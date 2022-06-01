#include <ostypes.h>
#include <stdarg.h>
#include <stdio.h>

#include <os_api.h>
#include <sg_ivector.h>

#include "qemu-versatilepb.h"


static inline u32 get_isr_mask_bits(int irqn) {
	u32 msk_bits = 0;
	if (irqn == ISR_SN_TIMER01) {
		/* get Masked Int. Status for Timer 0 */
		msk_bits = *((volatile u32*)(TIMER0_BASE+TIMERMSKINTSTS_OFFSET));
	}

	return msk_bits;
}

static inline void acknowledge_isr(int irqn) {
	if ((irqn > MAX_IVECTOR_NUMBER) || (irqn < MIN_IVECTOR_NUMBER)) {
		return;
	}

	if (irqn == ISR_SN_TIMER01) {
		/* acknowledge timer 0 interrupt */
		*((volatile u8 *)(TIMER0_BASE + TIMERINTCLR_OFFSET)) = 0;
	}
}

void __attribute__((interrupt)) irq_handler() {
	int iv = MIN_IVECTOR_NUMBER;
	do {
		/* continue if iv ISR has not occured */
		if (!(VIC_IRQSTATUS & (1 << iv)))
			continue;

		if (get_isr_mask_bits(iv)) {
			acknowledge_isr(iv);
			(*_IsrHandler[iv])();
		}
		iv++;
	}while (iv <= MAX_IVECTOR_NUMBER);
}



void __attribute__((interrupt)) undef_handler(void) {
	pr_log("\nEntered a trap function: %s()\n", __func__);
	for (;;);
}

void __attribute__((interrupt)) swi_handler(void) {
	pr_log("\nEntered a trap function: %s()\n", __func__);
	for (;;);
}

void __attribute__((interrupt)) prefetch_abort_handler(void) {
	pr_log("\nEntered a trap function: %s()\n", __func__);
	for (;;);
}

void __attribute__((interrupt)) data_abort_handler(void) {
	pr_log("\nEntered a trap function: %s()\n", __func__);
	for (;;);
}

void __attribute__((interrupt)) fiq_handler(void) {
	pr_log("\nEntered a trap function: %s()\n", __func__);
	for (;;);
}

void __copy_vectors(void) {
	extern uint32_t _vectors_start;
	extern uint32_t _vectors_end;
	uint32_t *vectors_src = &_vectors_start;
	uint32_t *vectors_dst = (uint32_t *)0;

	while (vectors_src < &_vectors_end)
		*vectors_dst++ = *vectors_src++;
}