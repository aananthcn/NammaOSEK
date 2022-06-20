#ifndef _QEMU_VERSATILEPB
#define _QEMU_VERSATILEPB

#include <ostypes.h>
#include <platform.h>

#define QEMU_VERSATILEPB	1

int brd_setup_sytimer(void);
int brd_get_usec_syscount(u32 *ucount);

#endif