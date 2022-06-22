#include <ostypes.h>

#include "rp2040.h"


extern u32 __set_interrupt_bits(u32 isr_bits, u32 addr);
extern u32 __clear_interrupt_bits(u32 isr_bits, u32 addr);



u32 _set_interrupt_bits(u32 bits) {
	u32 retval;
	u32 nvic_iser = PPB_BASE + NVIC_ISER_OFFSET;

	retval = __set_interrupt_bits(bits, nvic_iser);

	return retval;
}


u32 _clear_interrupt_bits(u32 bits) {
	u32 retval;
	u32 nvic_iser = PPB_BASE + NVIC_ISER_OFFSET;

	retval = __clear_interrupt_bits(bits, nvic_iser);

	return retval;
}