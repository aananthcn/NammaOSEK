#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>

#include <osek.h>

#include "board.h"

void isr_handler(int unused) {
    SetActiveApplicationMode(OS_SLEEP_MODE);
}


int main(void)
{
    setup_systick();
    signal(SIGINT, isr_handler);
    
    StartOS(OSDEFAULTAPPMODE);
    /* The execution should never reach here */
    printf("Info: StartOS() function returned!! OS Exits!\n");

    return 0;
}