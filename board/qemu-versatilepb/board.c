#include <ostypes.h>
#include <stdarg.h>
#include <stdio.h>

#include <os_api.h>


volatile unsigned int *const UART0DR = (unsigned int *)0x101f1000;

int console_fputs(const char *s) {
	int count = 0;

	while (*s != '\0') {
		*UART0DR = (unsigned int)(*s);
		count++;
		s++;
	}

	return count;
}

int console_fputc(const int c) {
	*UART0DR = (unsigned int) (c);
	return c;
}


void c_entry() {
	static bool hello_print = false;

	if (hello_print == false) {
		pr_log("Hello FreeOSEK!\n");
		hello_print = true;
	}
}

int brd_setup_sytimer(void) {
	return 0;
}

int brd_get_usec_syscount(u32 *ucount) {
	return 0;
}

#if 0
int printf(const char* format, ...)
{
	va_list vargs;
	int     r;

	va_start(vargs, format);
	//r = cbvprintf(fputc, DESC(stdout), format, vargs);
	r = console_fputs(format);
	va_end(vargs);

	return r;
}

int vfprintf(FILE* stream, const char* format, va_list vargs)
{
	int r;

	//r = cbvprintf(fputc, DESC(stream), format, vargs);
	r = console_fputs(format);

	return r;
}
#endif
