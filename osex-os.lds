/*
 * Author: Aananth C N
 * Date: 27 Jun 2021
 */

OUTPUT_FORMAT ("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)

/* Memory Map for STM32F401RE from datasheet */
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
     */
    . = ORIGIN(FLASH);

    /*
     * First link the vector table.
     */
    .vectors : ALIGN(4)
    {
        FILL(0xFF)
        __vectors_start__ = ABSOLUTE(.); 
        KEEP(*(.vectors))
        *(.after_vectors .after_vectors.*)
    } > FLASH

    /*
     * Start of text.
     */
    _text = .;

    /*
     * Text section
     */
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
     */
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    /*
     * Arm stack unwinding.
     * If removed may cause random crashes.
     */
    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > FLASH

    /*
     * Section used by C++ to access eh_frame.
     * Generaly not used, but it doesn't harm to be there.
     */ 
    .eh_frame_hdr :
    {
        *(.eh_frame_hdr)
    } > FLASH

    /*
     * Stack unwinding code.
     * Generaly not used, but it doesn't harm to be there.
     */ 
    .eh_frame : ONLY_IF_RO
    {
        *(.eh_frame)
    } > FLASH

    /*
     * Read-only data. Consts should also be here.
     */
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
     */
    _etext = .;

    /*
     * Data section.
     */
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
     */
    .bss (NOLOAD) : ALIGN(4)
    {
        . = ALIGN(4);
        PROVIDE(_bss_start = .);
        __bss_start__ = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
        PROVIDE(_bss_end = .);
        __bss_end__ = .;
        PROVIDE(end = .);
    } > SRAM

    /*
     * Non-initialized variables section.
     * A variable should be explicitly placed
     * here, aiming in speeding-up boot time.
     */
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
    */
}