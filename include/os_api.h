#ifndef AANANTH_OSEK_OS_API_H
#define AANANTH_OSEK_OS_API_H

#include <base_types.h>
#include <stdio.h>



// Macros
#define ONE_MSEC_IN_MICROSEC	(1000)


// Public functions
u32 _GetOsTickCnt(void);
int _OsHandleTicks(void);


// /* low level APIs  - Stack Pointer Manipulation functions */
// u32 _set_stack_ptr(u32 new_stack);
// u32 _get_stack_ptr(void);
// u32 _get_irq_stack_ptr(void);
// u32 _get_next_pc(void);
// void _set_sp_and_pc(u32 sp, u32 pc);
// u32 _save_context(u32 sp);
// void _restore_context(u32 sp);

/* low level APIs  - Interrupt functions */
// u32 _set_interrupt_bits(u32 if_bits);
// u32 _clear_interrupt_bits(u32 if_bits);


// int pr_log(const char *format, ...);
// int pr_log_init(void);
#define pr_log 	printf


#endif