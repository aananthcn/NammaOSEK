#include <config.h>


/**
 * @def Z_KERNEL_STACK_ARRAY_DEFINE_IN
 * @brief Define a toplevel array of kernel stack memory regions in specified section
 *
 * @param sym Kernel stack array symbol name
 * @param nmemb Number of stacks to declare
 * @param size Size of the stack memory region
 * @param lsect Linker section for this array of stacks
 */
#define Z_KERNEL_STACK_ARRAY_DEFINE_IN(sym, nmemb, size, lsect) \
	struct z_thread_stack_element lsect \
		__aligned(Z_KERNEL_STACK_OBJ_ALIGN) \
		sym[nmemb][Z_KERNEL_STACK_LEN(size)]
#define K_KERNEL_PINNED_STACK_ARRAY_DEFINE(sym, nmemb, size) \
	Z_KERNEL_STACK_ARRAY_DEFINE_IN(sym, nmemb, size, __kstackmem)


/*
 * storage space for the interrupt stack
 *
 * Note: This area is used as the system stack during kernel initialization,
 * since the kernel hasn't yet set up its own stack areas. The dual purposing
 * of this area is safe since interrupts are disabled until the kernel context
 * switches to the init thread.
 */
K_KERNEL_PINNED_STACK_ARRAY_DEFINE(z_interrupt_stacks,
				   CONFIG_MP_NUM_CPUS,
				   CONFIG_ISR_STACK_SIZE);


void z_cstart(void)
{
	// cstartup
	arch_kernel_init();

	while (1) {
		// do nothing!
	}
}
