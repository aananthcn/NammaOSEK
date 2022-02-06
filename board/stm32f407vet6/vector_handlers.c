#include <ostypes.h>
#include <stdarg.h>
#include <stdio.h>

#include <os_api.h>
#include <sg_ivector.h>

#include "stm32f407vet6.h"
#include "cortex-m4.h"


/* Exception no: 1 */
extern void _Reset_handler(void);
/* _Reset_handler is implemented in startup.s */

/* Exception no: 2 */
void __attribute__((interrupt)) _NMI_handler(void) {
        pr_log("\nEntered a trap function: %s()\n", __func__);
        for (;;);
}

/* Exception no: 3 */
void __attribute__((interrupt)) _HardFault_handler(unsigned int* pStack) {
        pr_log("\nEntered a trap function: %s()\n", __func__);

        if (NVIC_HFSR & (1uL << 31)) {
                NVIC_HFSR |= (1uL << 31);       // Reset Hard Fault status
                *(pStack + 6u) += 2u;           // PC is located on stack at SP + 24 bytes;                               
                                                // increment PC by 2 to skip break instruction.
                return;                         // Return to interrupted application
        }
#if DEBUG
        //
        // Read NVIC registers
        //
        HardFaultRegs.syshndctrl.byte = SYSHND_CTRL;    // System Handler Control and State Register
        HardFaultRegs.mfsr.byte = NVIC_MFSR;            // Memory Fault Status Register
        HardFaultRegs.bfsr.byte = NVIC_BFSR;            // Bus Fault Status Register
        HardFaultRegs.bfar = NVIC_BFAR;                 // Bus Fault Manage Address Register
        HardFaultRegs.ufsr.byte = NVIC_UFSR;            // Usage Fault Status Register
        HardFaultRegs.hfsr.byte = NVIC_HFSR;            // Hard Fault Status Register
        HardFaultRegs.dfsr.byte = NVIC_DFSR;            // Debug Fault Status Register
        HardFaultRegs.afsr = NVIC_AFSR;                 // Auxiliary Fault Status Register
        
        //
        // Halt execution
        // If NVIC registers indicate readable memory, change the variable value
        // to != 0 to continue execution.
        //
        _Continue = 0u;
        while (_Continue == 0u)
                ;

        //
        // Read saved registers from the stack
        //
        HardFaultRegs.SavedRegs.r0       = pStack[0];  // Register R0  
        HardFaultRegs.SavedRegs.r1       = pStack[1];  // Register R1  
        HardFaultRegs.SavedRegs.r2       = pStack[2];  // Register R2  
        HardFaultRegs.SavedRegs.r3       = pStack[3];  // Register R3  
        HardFaultRegs.SavedRegs.r12      = pStack[4];  // Register R12  
        HardFaultRegs.SavedRegs.lr       = pStack[5];  // Link register LR  
        HardFaultRegs.SavedRegs.pc       = pStack[6];  // Program counter PC  
        HardFaultRegs.SavedRegs.psr.byte = pStack[7];  // Program status word PSR
        
        //
        // Halt execution
        // To step out of the HardFaultHandler, change the variable value to != 0.
        //
        _Continue = 0u;
        while (_Continue == 0u)
        {
        }
#else //
        // If this module is included in a release configuration,
        // simply stay in the HardFault handler
        //
        (void)pStack;
        do
        {
        } while (1);
#endif
}

/* Exception no: 4 */
void __attribute__((interrupt)) _MemManage_handler(void) {
        pr_log("\nEntered a trap function: %s()\n", __func__);
        for (;;);
}

/* Exception no: 5 */
void __attribute__((interrupt)) _BusFault_handler(void) {
        pr_log("\nEntered a trap function: %s()\n", __func__);
        for (;;);
}

/* Exception no: 6 */
void __attribute__((interrupt)) _UsageFault_handler(void) {
        pr_log("\nEntered a trap function: %s()\n", __func__);
        for (;;);
}

/* Exception no: 11 */
void __attribute__((interrupt)) _SVCall_handler(void) {
        pr_log("\nEntered a trap function: %s()\n", __func__);
        for (;;);
}

/* Exception no: 12 */
void __attribute__((interrupt)) _DebugMonitor_handler(void) {
        pr_log("\nEntered function: %s()\n", __func__);
        _Reset_handler();
}

/* Exception no: 14 */
void __attribute__((interrupt)) _PendSV_handler(void) {
        pr_log("\nEntered a trap function: %s()\n", __func__);
        for (;;);
}

extern void SystemTickISR(void);
/* Exception no: 15 */
void __attribute__((interrupt)) _SysTick_handler(void) {
        SystemTickISR();
}

///////////////////////////////////////////////////////////////////////////////
// Exception no: 16
void __attribute__((interrupt)) _irq_handler() {
        pr_log("\nEntered a trap function: %s()\n", __func__);
        for (;;);
}



/*
  The following function doesn't need for Cortex M4
*/
#if 0
void __copy_vectors(void) {
        extern uint32_t _vectors_start;
        extern uint32_t _vectors_end;
        uint32_t *vectors_src = &_vectors_start;
        uint32_t *vectors_dst = (uint32_t *)0;

        while (vectors_src < &_vectors_end)
                *vectors_dst++ = *vectors_src++;
}
#endif