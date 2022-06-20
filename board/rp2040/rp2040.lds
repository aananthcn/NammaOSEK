/*
 * Author: Aananth C N
 * Date: 21 Nov 2021
 */

INCLUDE tools/src/sg_stack.lds

MEMORY
{
    FLASH(rx)       : ORIGIN = 0x10000000, LENGTH = 2048k
    RAM(rwx)        : ORIGIN =  0x20000000, LENGTH = 256k
    SCRATCH_X(rwx)  : ORIGIN = 0x20040000, LENGTH = 4k
    SCRATCH_Y(rwx)  : ORIGIN = 0x20041000, LENGTH = 4k
}

/* Flash Memory: 2 MB */
/*
BOOT_STG2     = 0x10000000;
ROM_START    = 0x10000100;
ROM_SIZE     = 0x00200000;
*/
/* RAM Memory: 256kB + 4kB + 4kB = 264kB */
/*
RAM_START    = 0x20000000;
RAM_SIZE     = 0x00042000;
*/

ENTRY(_Reset_handler)
SECTIONS
{
    /* Stage 2 Booloader for Raspbery Pi Pico (RP2040) */
    .boot2 : {
        _boot_stage2_start = .;
        KEEP (*(.boot2))
        _boot_stage2_end = .;
    } > FLASH

    ASSERT(_boot_stage2_end - _boot_stage2_start == 256,
        "ERROR: Pico second stage bootloader must be 256 bytes in size")

    /* Other CODE SEGMENTS -- place vectors.o first?? */
    .text : {
        _vectors_start = .;
        KEEP (*(.vectors))
        _vectors_end = .;
        . = ALIGN(4);
        *(.text)
        . = ALIGN(4);
        *(.rodata)
    } > FLASH

    /*
     * Arm stack unwinding.
     * If removed may cause random crashes.
     */
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH
    
    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;

    _init_data_rom = .;

    .ram_vector_table (COPY): {
        *(.ram_vector_table)
    } > RAM

    /* INITIALIZED DATA SEGMENT(s) */
    .data : AT (_init_data_rom)
    { 
        _data_start = .;
        *(.data) 
        _data_end = .;
    } > RAM 
    _data_size = _data_end - _data_start;

    /* UNINITIALIZED DATA SEGMENT(s) */
    .bss : 
    { 
        _bss_start = .;
        *(.bss COMMON)
        _bss_end = .;
    } > RAM
    _bss_size = _bss_end - _bss_start;

    . = ALIGN(8);
    /* OS SCRATCH PAD AREA */
    . = . + 64;
    _os_scratch_pad = .;
    
    /* USER STACK MEMORY */
    . = . + _TASK_STACK_SIZE; /* user stack memory */
    _user_stack_top = .;

    /* OS STACK MEMORY */
    . = . + _OS_STACK_SIZE; /* user stack memory */
    _os_stack_top = .;

    /* IRQ STACK MEMORY */
    . = . + _IRQ_STACK_SIZE; /* 4kB of irq stack memory */
    _irq_stack_top = .;
}
