.text
.cpu cortex-m4
.syntax unified
.thumb
 
.global _set_interrupt_bits
.global _clear_interrupt_bits


/*/
ARM Current Program Status Register (CPSR)
---------------------------------------------------------------------------------------------
Bits      | Name    | Function
---------------------------------------------------------------------------------------------
[31]      | N       | Negative condition code flag
[30]      | Z       | Zero condition code flag
[29]      | C       | Carry condition code flag
[28]      | V       | Overflow condition code flag
[27]      | Q       | Cumulative saturation bit
[26:25]   | IT[1:0] | If-Then execution state bits for the Thumb IT (If-Then) instruction
[24]      | J       | Jazelle bit
[19:16]   | GE      | Greater than or Equal flags
[15:10]   | IT[7:2] | If-Then execution state bits for the Thumb IT (If-Then) instruction
[9]       | E       | Endianness execution state bit: 0 - Little-endian, 1 - Big-endian
[8]       | A       | Asynchronous abort mask bit
[7]       | I       | IRQ mask bit (Disables IRQ interrupts when it is set.)
[6]       | F       | FIRQ mask bit (Disables FIQ interrupts when it is set.)
[5]       | T       | Thumb execution state bit
[4:0]     | M       | Mode field
---------------------------------------------------------------------------------------------
/*/


/*/
Function Name: _set_interrupt_bits
Arguments: arg1 = 32 bit integer with FIRQ|IRQ bits set or cleared.
Returns: previous CPSR register value.
Description: This function sets the FIRQ and IRQ bits as passed as arguments.
             The users should noted that this function expects FIRQ and IRQ
             values to be passed at 6th and 7th bit position respectively.
/*/
 _set_interrupt_bits:
        and r0, r0, #0xC0       @ clear bits other than 6th and 7th
        Todo: Fix this
        //mrs r1, cpsr
        //orr r0, r1, r0
        //msr cpsr, r0
        //mov r0, r1
        mov pc, lr



/*/
Function Name: _clear_interrupt_bits
Arguments: arg1 = 32 bit integer with FIRQ|IRQ bits set or cleared.
Returns: previous CPSR register value.
Description: This function clears the FIRQ and IRQ bits as passed as arguments.
             The users should noted that this function expects FIRQ and IRQ
             values to be passed at 6th and 7th bit position respectively.
/*/
 _clear_interrupt_bits:
        and r0, r0, #0xC0       @ clear bits other than 6th and 7th
        Todo: Fix this
        //mrs r2, cpsr
        //bic r1, r2, #0xC0       @ clear bits other than 6th and 7th
        //orr r0, r1, r0
        //msr cpsr, r0
        //mov r0, r2
        mov pc, lr

.end
