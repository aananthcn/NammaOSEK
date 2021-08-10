#include <stdio.h>
#include <stdarg.h>
#include <board.h>

static u32 usec_0;

int pr_log(const char *format, ...)
{
	va_list arg;
	int done;
	va_list time;
	u32 usec;

	if (0 == brd_get_usec_syscount(&usec)) {
		usec = usec - usec_0;
		printf("[%d.%06d] ", usec/1000000, usec%1000000);
	}

	va_start(arg, format);
	done = vfprintf(stdout, format, arg);
	va_end(arg);

	return done;
}


int pr_log_init(void) {
	int ret = -1;

	if (0 == brd_get_usec_syscount(&usec_0)) {
		ret = 0;
	}

	pr_log("Welcome to FreeOSEK!\n");
	return ret;
}
