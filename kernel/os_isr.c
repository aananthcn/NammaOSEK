#include <stdio.h>


#include <os_api.h>


u32 _OsIsrBits;

#define DISABLE_INTERRUPT	(0xC0)
#define ENABLE_INTERRUPT	(0)


/*/
Function: DisableAllInterrupts
Parameters: none

Description: This service disables all interrupts for which the hardware
	     supports disabling. The state before is saved for the
	     EnableAllInterrupts call.
/*/
void DisableAllInterrupts(void) {
	_OsIsrBits = _set_interrupt_bits(DISABLE_INTERRUPT);
}



/*/
Function: EnableAllInterrupts
Parameters: none

Description: This service restores the state saved by DisableAllInterrupts.
/*/
void EnableAllInterrupts(void) {
	_OsIsrBits = _clear_interrupt_bits(_OsIsrBits);
}