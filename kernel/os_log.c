#include <stdio.h>
#include <stdarg.h>
#include <board.h>


int pr_log(const char *format, ...)
{
	va_list arg;
	int done;
	va_list time;
	u32 usec;

	if (0 == brd_get_usec_syscount(&usec)) {
		printf("[%d.%06d] ", usec/1000000, usec%1000000);
	}

	va_start(arg, format);
	done = vfprintf(stdout, format, arg);
	va_end(arg);

	return done;
}