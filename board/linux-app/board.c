#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>

#include <osek.h>

void isr_handler(int unused) {
    SetActiveApplicationMode(OS_SHUTDOWN_MODE);
}


int BoardSpecific_BackgroundTask(void) {
    sleep(1);
    printf("%s(): completed!\n", __func__);
}


int main(void)
{
    signal(SIGINT, isr_handler);
    
    StartOS(OSDEFAULTAPPMODE);
    /* The execution should never reach here */
    printf("Info: StartOS() function returned!! OS Exits!\n");

    return 0;
}