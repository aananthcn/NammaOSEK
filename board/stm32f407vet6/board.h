#ifndef _BOARD_H
#define _BOARD_H

#include <ostypes.h>
#include <boards.h>

#define BOARD STM32F407VET6

int brd_setup_sytimer(void);
int brd_get_usec_syscount(u32 *ucount);

#endif