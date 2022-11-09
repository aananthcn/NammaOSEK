#include <base_types.h>

static u32 errno;

u32* free_osek_errno(void) {
	return &errno;
}