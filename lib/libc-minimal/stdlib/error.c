#include <ostypes.h>

static u32 errno;

u32* free_osek_errno(void) {
	return &errno;
}