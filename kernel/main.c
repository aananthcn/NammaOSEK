#include <config.h>
#include <compiler.h>
#include <stack.h>


/* Using typedef deliberately here, this is quite intended to be an opaque
 * type.
 *
 * The purpose of this data type is to clearly distinguish between the
 * declared symbol for a stack (of type k_thread_stack_t) and the underlying
 * buffer which composes the stack data actually used by the underlying
 * thread; they cannot be used interchangeably as some arches precede the
 * stack buffer region with guard areas that trigger a MPU or MMU fault
 * if written to.
 *
 * APIs that want to work with the buffer inside should continue to use
 * char *.
 *
 * Stacks should always be created with K_THREAD_STACK_DEFINE().
 */
struct __packed z_thread_stack_element {
	char data;
};

#define Z_KERNEL_STACK_OBJ_ALIGN	4
#define ARCH_STACK_PTR_ALIGN 		4
#define K_KERNEL_STACK_RESERVED		0
#define ROUND_UP(x, align)                                   \
	(((unsigned long)(x) + ((unsigned long)(align) - 1)) & \
	 ~((unsigned long)(align) - 1))
#define Z_KERNEL_STACK_SIZE_ADJUST(size) (ROUND_UP(size, \
						   ARCH_STACK_PTR_ALIGN) + \
					  K_KERNEL_STACK_RESERVED)
#define Z_KERNEL_STACK_LEN(size) \
	ROUND_UP(Z_KERNEL_STACK_SIZE_ADJUST(size), Z_KERNEL_STACK_OBJ_ALIGN)


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

#define __pinned_noinit __in_section_unique(task_stack)

#define K_KERNEL_PINNED_STACK_ARRAY_DEFINE(sym, nmemb, size) \
	Z_KERNEL_STACK_ARRAY_DEFINE_IN(sym, nmemb, size, __pinned_noinit)


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


#include <kernel_arch_func.h>

void z_cstart(void)
{
	// cstartup
	arch_kernel_init();

	while (1) {
		// do nothing!
	}
}
