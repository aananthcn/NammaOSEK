/*
 * Author: Aananth C N
 * Date: 21 Nov 2021
 */

INCLUDE tools/src/sg_stack.lds


/* Flash Memory: 2 MB */
ROM_START    = 0x10000000;
ROM_SIZE     = 0x00200000;

/* RAM Memory: 256kB + 4kB + 4kB = 264kB */
RAM_START    = 0x20000000;
RAM_SIZE     = 0x00042000;


ENTRY(_vectors_start)
SECTIONS
{
    /* LOAD ADDRESS - The -kernel option (to qemu) loads a binary file inside
    the system memory starting at address ROM_START */
    . = ROM_START;

    /* CODE SEGMENTS -- place vectors.o first! */
    .text : {
        _vectors_start = .;
        board/rp2040/vectors.o
        _vectors_end = .;
        *(.text .rodata)
    }

    /*
     * Arm stack unwinding.
     * If removed may cause random crashes.
     */
    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    }

    _init_data_rom = .;

    . = RAM_START;
    /* INITIALIZED DATA SEGMENT(s) */
    .data : AT (_init_data_rom)
    { 
        _data_start = .;
        *(.data) 
        _data_end = .;
    }
    _data_size = _data_end - _data_start;

    /* UNINITIALIZED DATA SEGMENT(s) */
    .bss : 
    { 
        _bss_start = .;
        *(.bss COMMON)
        _bss_end = .;
    }
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
