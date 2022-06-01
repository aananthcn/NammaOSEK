#ifndef _BOARD_STM32F407VET6
#define _BOARD_STM32F407VET6

#include <ostypes.h>
#include <platform.h>

#define BOARD_STM32F407VET6	1

int brd_setup_sytimer(void);
int brd_get_usec_syscount(u32 *ucount);

#endif