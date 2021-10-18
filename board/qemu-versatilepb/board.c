#include <ostypes.h>
#include <stdarg.h>
#include <stdio.h>

volatile unsigned int *const UART0DR = (unsigned int *)0x101f1000;

int print_uart0(const char *s) {
	int count = 0;

	while (*s != '\0') {
		*UART0DR = (unsigned int)(*s);
		count++;
		s++;
	}

	return count;
}

void c_entry() {
	print_uart0("Hello FreeOSEK!\n");
}

int brd_setup_sytimer(void) {
	return 0;
}

int brd_get_usec_syscount(u32 *ucount) {
	return 0;
}

int printf(const char* format, ...)
{
	va_list vargs;
	int     r;

	va_start(vargs, format);
	//r = cbvprintf(fputc, DESC(stdout), format, vargs);
	r = print_uart0(format);
	va_end(vargs);

	return r;
}

int vfprintf(FILE* stream, const char* format, va_list vargs)
{
	int r;

	//r = cbvprintf(fputc, DESC(stream), format, vargs);
	r = print_uart0(format);

	return r;
}
