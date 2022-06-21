.syntax unified
.cpu cortex-m0plus
.thumb
.align 2

.text
 
.global _set_stack_ptr
.global _get_stack_ptr
.global _get_next_pc
.global _set_sp_and_pc
.global _save_context
.global _restore_context


/*/
 * Function Name: _set_stack_ptr
 * Arguments: arg1 (r0) = new_stack_ptr
 * Returns: old stack pointer
 * Description: This function sets stack pointer of the uC with the argument 
 *              passed. 
 * Return value: This returns the stack pointer before setting the new one.
/*/
_set_stack_ptr:
	mov r2, sp
	mov sp, r0
	mov r0, r2
	mov pc, lr



/*/
 * Function Name: _get_stack_ptr
 * Arguments: arg1 = none
 * Returns: current stack pointer
 * Description: This function returns the current stack pointer value.
/*/
_get_stack_ptr:
	mov r0, sp
	mov pc, lr



/*/
 * Function Name: _get_next_pc
 * Arguments: arg1 = none
 * Returns: link register
 * Description: This function returns the next instruction to be executed from
 *              the point this function was called from.
/*/
_get_next_pc:
	mov r0, lr 
	mov pc, lr



/*/ 
 * Function Name: _set_sp_and_pc
 * Arguments: arg1 (r0) = sp, arg2 (r1) = pc 
 * Returns: The new program counter value
 * Description: This function sets the stack pointer and program counter to the
 *              values passed as arguments
/*/
_set_sp_and_pc:
	mov sp, r0
	mov r0, r1
	mov pc, r1



/*/ 
 * Function Name: _save_context
 * Arguments: arg1 (r0) = location where the context to be saved.
 * Returns: none
 * Description: This function saves the context to the location pointed by the
 *              argument passed.
/*/
_save_context:
	// stmia   r0!, {r4 - sl, fp, sp, lr}   @ sl = r10, stack limit
	mov r1, sp @ saving SP to R1 so that it can be pushed to stack using push instruction.
	mov sp, r0
	push {r4 - r7}    @ Thumb-Instruction-Set can use r0-r7, r13, r14 and r15 register.
	push {lr}
	push {r1} @ push original stack pointer
	mov sp, r1 @ I am not sure if this is needed, but added as context will not be switched in this function.
	mov pc, lr



/*/ 
 * Function Name: _restore_context
 * Arguments: arg1 (r0) = location where the context to be restored from.
 * Returns: none
 * Description: This function restores the context to the location pointed by 
 *              the argument passed.
/*/
_restore_context:
	//ldmia   r0!, {r4 - sl, fp, sp, lr}      @ Load all regs saved previously
	mov sp, r0
	pop {r4 - r7}     @ Load all low regs saved previously
	pop {r1}
	mov lr, r1
	pop {r1}
	mov sp, r1
	mov pc, lr


.end
