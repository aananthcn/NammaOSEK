.text
 .code 32
 
.global _set_stack_ptr
.global _get_stack_ptr
.global _get_next_pc
.global _set_sp_and_pc

/*/ 
 * Function Name: _set_stack_ptr
 * Arguments: arg1 = new_stack_ptr
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
 * Description: This function returns the current stack pointer value.
/*/
_get_stack_ptr:
	mov r0, sp
	mov pc, lr



/*/ 
 * Function Name: _get_next_pc
 * Arguments: arg1 = none
 * Description: This function returns the next instruction to be exec. from
 *              the point this function was called from.
/*/
_get_next_pc:
	mov r0, lr 
	mov pc, lr



/*/ 
 * Function Name: _set_sp_and_pc
 * Arguments: arg1 = sp, arg2 = pc 
 * Description: This function sets the stack pointer and program counter to the
 *              values passed as arguments
/*/
_set_sp_and_pc:
	mov sp, r0
	mov r0, r1
	mov pc, r1


.end
