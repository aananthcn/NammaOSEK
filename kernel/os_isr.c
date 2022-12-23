#include <stdio.h>

#include <bsp/platform.h>
#include <os_api.h>


u32 _OsIsrBitsSaved;



/*/
Function: DisableAllInterrupts
Parameters: none

Description: This service disables all interrupts for which the hardware
	     supports disabling. The state before is saved for the
	     EnableAllInterrupts call.
/*/
void DisableAllInterrupts(void) {
	_OsIsrBitsSaved = _set_interrupt_bits(DISABLE_INTERRUPT);
}



/*/
Function: EnableAllInterrupts
Parameters: none

Description: This service restores the state saved by DisableAllInterrupts.
/*/
void EnableAllInterrupts(void) {
	_OsIsrBitsSaved = _clear_interrupt_bits(ENABLE_INTERRUPT);
}


/* Following OSEK APIs are not 100% correct, but this should work! */
void SuspendAllInterrupts(void) {
	_OsIsrBitsSaved = _set_interrupt_bits(DISABLE_INTERRUPT);
}
void SuspendOSInterrupts(void) {
	_OsIsrBitsSaved = _set_interrupt_bits(DISABLE_INTERRUPT);
}

void ResumeAllInterrupts(void) {
	_OsIsrBitsSaved = _clear_interrupt_bits(_OsIsrBitsSaved);
}
void ResumeOSInterrupts(void) {
	_OsIsrBitsSaved = _clear_interrupt_bits(_OsIsrBitsSaved);
}
