#include <config.h>
#include <compiler.h>
#include <stack.h>
#include <string.h>


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



#define Z_THREAD_STACK_OBJ_ALIGN(size)	ARCH_STACK_PTR_ALIGN
#define K_THREAD_STACK_RESERVED		((size_t)0U)
#define Z_THREAD_STACK_SIZE_ADJUST(size) \
	(ROUND_UP((size), ARCH_STACK_PTR_ALIGN) + K_THREAD_STACK_RESERVED)
/**
 * @brief Declare a toplevel thread stack memory region in specified region
 *
 * This declares a region of memory suitable for use as a thread's stack
 * in specified region.
 *
 * This is the generic, historical definition. Align to Z_THREAD_STACK_OBJ_ALIGN
 * and put in 'noinit' section so that it isn't zeroed at boot
 *
 * The declared symbol will always be a k_thread_stack_t which can be passed to
 * k_thread_create(), but should otherwise not be manipulated. If the buffer
 * inside needs to be examined, examine thread->stack_info for the associated
 * thread object to obtain the boundaries.
 *
 * It is legal to precede this definition with the 'static' keyword.
 *
 * It is NOT legal to take the sizeof(sym) and pass that to the stackSize
 * parameter of k_thread_create(), it may not be the same as the
 * 'size' parameter. Use K_THREAD_STACK_SIZEOF() instead.
 *
 * Some arches may round the size of the usable stack region up to satisfy
 * alignment constraints. K_THREAD_STACK_SIZEOF() will return the aligned
 * size.
 *
 * @param sym Thread stack symbol name
 * @param size Size of the stack memory region
 * @param lsect Linker section for this stack
 */
#define Z_THREAD_STACK_DEFINE_IN(sym, size, lsect) \
	struct z_thread_stack_element lsect \
		__aligned(Z_THREAD_STACK_OBJ_ALIGN(size)) \
		sym[Z_THREAD_STACK_SIZE_ADJUST(size)]

/**
 * @brief Declare a toplevel thread stack memory region
 *
 * This declares a region of memory suitable for use as a thread's stack.
 *
 * This is the generic, historical definition. Align to Z_THREAD_STACK_OBJ_ALIGN
 * and put in 'noinit' section so that it isn't zeroed at boot
 *
 * The declared symbol will always be a k_thread_stack_t which can be passed to
 * k_thread_create(), but should otherwise not be manipulated. If the buffer
 * inside needs to be examined, examine thread->stack_info for the associated
 * thread object to obtain the boundaries.
 *
 * It is legal to precede this definition with the 'static' keyword.
 *
 * It is NOT legal to take the sizeof(sym) and pass that to the stackSize
 * parameter of k_thread_create(), it may not be the same as the
 * 'size' parameter. Use K_THREAD_STACK_SIZEOF() instead.
 *
 * Some arches may round the size of the usable stack region up to satisfy
 * alignment constraints. K_THREAD_STACK_SIZEOF() will return the aligned
 * size.
 *
 * @param sym Thread stack symbol name
 * @param size Size of the stack memory region
 */
#define __stackmem __in_section_unique(main_stack)

#define K_THREAD_STACK_DEFINE(sym, size) \
	Z_THREAD_STACK_DEFINE_IN(sym, size, __stackmem)

K_THREAD_STACK_DEFINE(z_main_stack, CONFIG_MAIN_STACK_SIZE);



#include <kernel_arch_func.h>

void z_cstart(void)
{
	// cstartup
	arch_kernel_init();

	while (1) {
		// do nothing!
	}
}


/**
 *
 * @brief Handler installed in the vector table
 *
 * Simply call what is installed in 'static void(*handler)(void)'.
 *
 * @return N/A
 */

void z_arm_nmi(void)
{
	// do nothing
	// handler();
}


/**
 *
 * @brief Service call handler
 *
 * The service call (svc) is used in the following occasions:
 * - IRQ offloading
 * - Kernel run-time exceptions
 * - System Calls (User mode)
 *
 * @return N/A
 */
void z_arm_svc(void)
{
	// do nothing
}
