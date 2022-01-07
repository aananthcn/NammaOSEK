#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>

#include <osek.h>
#include <sg_appmodes.h>
#include <os_api.h>

#include "board.h"

void brd_isr_handler(int unused) {
    SetActiveApplicationMode(OS_MODES_MAX-1);
}


int main(void)
{
    pr_log_init();
    brd_setup_sytimer();
    signal(SIGINT, brd_isr_handler);
    
    StartOS(OSDEFAULTAPPMODE);
    /* The execution should never reach here */
    pr_log("Info: StartOS() function returned!! OS Exits!\n");

    return 0;
}