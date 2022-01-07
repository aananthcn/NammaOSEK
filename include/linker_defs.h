#ifndef INCLUDE_LINKER_DEFS_H
#define INCLUDE_LINKER_DEFS_H


/*
 * Memory owned by the kernel, to be used as shared memory between
 * application threads.
 *
 * The following are extern symbols from the linker. This enables
 * the dynamic k_mem_domain and k_mem_partition creation and alignment
 * to the section produced in the linker.

 * The policy for this memory will be to initially configure all of it as
 * kernel / supervisor thread accessible.
 */
extern char _app_smem_start[];
extern char _app_smem_end[];
extern char _app_smem_size[];
extern char _app_smem_rom_start[];
extern char _app_smem_num_words[];

/* Memory owned by the kernel. Start and end will be aligned for memory
 * management/protection hardware for the target architecture.
 *
 * Consists of all kernel-side globals, all kernel objects, all thread stacks,
 * and all currently unused RAM.
 *
 * Except for the stack of the currently executing thread, none of this memory
 * is normally accessible to user threads unless specifically granted at
 * runtime.
 */
extern char __kernel_ram_start[];
extern char __kernel_ram_end[];
extern char __kernel_ram_size[];

/* Used by z_bss_zero or arch-specific implementation */
extern char __bss_start[];
extern char __bss_end[];

/* Used by z_data_copy() or arch-specific implementation */
#ifdef CONFIG_XIP
extern char __data_rom_start[];
extern char __data_ram_start[];
extern char __data_ram_end[];
#endif /* CONFIG_XIP */

#ifdef CONFIG_MMU
/* Virtual addresses of page-aligned kernel image mapped into RAM at boot */
extern char z_mapped_start[];
extern char z_mapped_end[];
#endif /* CONFIG_MMU */

/* Includes text and rodata */
extern char _image_rom_start[];
extern char _image_rom_end[];
extern char _image_rom_size[];

/* Includes all ROMable data, i.e. the size of the output image file. */
extern char _flash_used[];

/* datas, bss, noinit */
extern char _image_ram_start[];
extern char _image_ram_end[];

extern char _image_text_start[];
extern char _image_text_end[];
extern char _image_text_size[];

extern char _image_rodata_start[];
extern char _image_rodata_end[];
extern char _image_rodata_size[];

extern char _vector_start[];
extern char _vector_end[];



#endif