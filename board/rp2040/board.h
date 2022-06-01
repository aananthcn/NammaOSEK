#ifndef _BOARD_RP2040
#define _BOARD_RP2040

#include <ostypes.h>
#include <platform.h>

#define BOARD_RP2040	1

int brd_setup_sytimer(void);
int brd_get_usec_syscount(u32 *ucount);

#endif