#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>

static volatile bool ExitCond = false;

void isr_handler(int unused) {
    ExitCond = true;
}

/* StartOS is defined in kernel.c */
extern void StartOS(void);

int main(void)
{
    while (!ExitCond) {
        StartOS();
        usleep(2000);
    }

    return 0;
}