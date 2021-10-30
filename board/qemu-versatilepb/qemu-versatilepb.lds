/*
 * Author: Aananth C N
 * Date: 21 Oct 2021
 */

INCLUDE tools/src/sg_stack.lds

ENTRY(vectors_start)
SECTIONS
{
    /* LOAD ADDRESS - The -kernel option (to qemu) loads a binary file inside
    the system memory starting at address 0x00010000 */
    . = 0x00010000;

    /* CODE SEGMENTS -- place vectors.o first! */
    .text : {
        vectors_start = .;
        board/qemu-versatilepb/vectors.o
        vectors_end = .;
        *(.text .rodata)
    }

    /* INITIALIZED DATA SEGMENT(s) */
    .data : 
    { 
        *(.data) 
    }

    /* UNINITIALIZED DATA SEGMENT(s) */
    .bss : 
    { 
        *(.bss COMMON)
    }

    . = ALIGN(8);
    /* STACK MEMORY */
    . = . + TASK_STACK_SIZE; /* 4kB of stack memory */
    stack_top = .;

    /* IRQ STACK MEMORY */
    . = . + IRQ_STACK_SIZE; /* 4kB of irq stack memory */
    irq_stack_top = .;
}

/*
OUTPUT_FORMAT ("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
*/

/* Memory Map for STM32F401RE from datasheet */
/*
ROM_ADDR = 0x08000000;
ROM_SIZE = 0x00080000;
RAM_ADDR = 0x20000000;
RAM_SIZE = 0x00018000;

MEMORY
{
  FLASH      (rx) : ORIGIN = ROM_ADDR, LENGTH = ROM_SIZE
  SRAM       (wx) : ORIGIN = RAM_ADDR, LENGTH = RAM_SIZE
} 

ENTRY(_start)

SECTIONS
{
    /* 
     * For normal programs should evaluate to 0, for placing the vector
     * table at the correct position.
     * /
    . = ORIGIN(FLASH);

    /*
     * First link the vector table.
     * /
    .vectors : ALIGN(4)
    {
        FILL(0xFF)
        _vector_start = .;
        KEEP(*(.exc_vector_table))
        KEEP(*(".exc_vector_table.*"))

        KEEP(*(_IRQ_VECTOR_TABLE_SECTION_SYMS))

        KEEP(*(.vectors))

        _vector_end = .;
    } > FLASH

    /*
     * Start of text.
     * /
    _text = .;

    /*
     * Text section
     * /
    .text : ALIGN(4)
    {
        *(.text)
        *(.text.*)
        *(.glue_7t)
        *(.glue_7)
        *(.gcc*)
    } > FLASH

    /*
     * Arm section unwinding.
     * If removed may cause random crashes.
     * /
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    /*
     * Arm stack unwinding.
     * If removed may cause random crashes.
     * /
    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > FLASH

    /*
     * Section used by C++ to access eh_frame.
     * Generaly not used, but it doesn't harm to be there.
     * / 
    .eh_frame_hdr :
    {
        *(.eh_frame_hdr)
    } > FLASH

    /*
     * Stack unwinding code.
     * Generaly not used, but it doesn't harm to be there.
     * / 
    .eh_frame : ONLY_IF_RO
    {
        *(.eh_frame)
    } > FLASH

    /*
     * Read-only data. Consts should also be here.
     * /
    .rodata : ALIGN(4)
    {
        . = ALIGN(4);
        __rodata_start__ = .;
        *(.rodata)
        *(.rodata.*)
        . = ALIGN(4);
        __rodata_end__ = .;
    } > FLASH 

    /*
     * End of text.
     * /
    _etext = .;

    /*
     * Data section.
     * /
    .data : ALIGN(4)
    {
        FILL(0xFF)
        . = ALIGN(4);
        PROVIDE(__textdata__ = LOADADDR(.data));
        PROVIDE(__data_start__ = .);
        *(.data)
        *(.data.*)
        *(.ramtext)
        . = ALIGN(4);
        PROVIDE(__data_end__ = .);
    } > SRAM AT > FLASH

    /*
     * BSS section.
     * /
    .bss (NOLOAD) : ALIGN(4)
    {
        . = ALIGN(4);
        PROVIDE(_bss_start = .);
        __bss_start = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
        PROVIDE(_bss_end = .);
        __bss_end = .;
        PROVIDE(end = .);
    } > SRAM

    /*
     * Non-initialized variables section.
     * A variable should be explicitly placed
     * here, aiming in speeding-up boot time.
     * /
    .noinit (NOLOAD) : ALIGN(4)
    {
        __noinit_start__ = .;
        *(.noinit .noinit.*) 
         . = ALIGN(4) ;
        __noinit_end__ = .;   
    } > SRAM

    /*
     * Heap section.
     */
    /*
    .heap (NOLOAD) :
    {
        . = ALIGN(4);
        __heap_start__ = .;
        __heap_base__ = .;
        . = ORIGIN(HEAP_RAM) + LENGTH(HEAP_RAM);
        __heap_end__ = .;
    } > SRAM
    * /
}
*/