COMPILER=arm-none-eabi-
CC=${COMPILER}gcc
LD=${COMPILER}ld
AS=${COMPILER}as
OBJCOPY=${COMPILER}objcopy
ARCH = arm32

INCDIRS  := -I ${CWD}/include \
            -I ${CWD}/include/arch/aarch32/ \
	    -I ${CWD}/board/ble-carbon \
	    -I ${CWD}/include/arch/aarch32/cortex_m/ \
	    -I ${CWD}/include/arch/aarch32/cortex_m/cmsis/ \
	    -I ${CWD}/lib/include

LDFLAGS  += -nostdlib -g
CFLAGS   += -Werror ${INCDIRS} -g
ASFLAGS  += ${INCDIRS} -g

$(info compilating for "-m armelf")
#CFLAGS  += -march=armv7-m -mcpu=cortex-m4
#ASFLAGS += -march=armv7-m -mcpu=cortex-m4
#CFLAGS  += -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 
#ASFLAGS += -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 
#CFLAGS  += -mthumb -march=armv7e-m -mfloat-abi=hard -mfpu=fpv4-sp-d16 
#ASFLAGS += -mthumb -march=armv7e-m -mfloat-abi=hard -mfpu=fpv4-sp-d16 
CFLAGS  += -mthumb -march=armv7e-m -mtune=cortex-m4 
ASFLAGS += -mthumb -march=armv7e-m -mtune=cortex-m4 
LDFILE	:= ${CWD}/board/ble-carbon/ble-carbon.lds
LDFLAGS += -m armelf -T ${LDFILE}


STDLIBOBJS	:= \
	lib/libc-minimal/stdlib/abort.o \
	lib/libc-minimal/stdlib/atoi.o \
	lib/libc-minimal/stdlib/bsearch.o \
	lib/libc-minimal/stdlib/exit.o \
	lib/libc-minimal/stdlib/malloc.o \
	lib/libc-minimal/stdlib/strtol.o \
	lib/libc-minimal/stdlib/strtoul.o \

LIBOBJS	:= \
	lib/libc-minimal/string/string.o

BRD_OBJS	:= \
	arch/arm32/cortex_m/reset.o \
	arch/arm32/cortex_m/vector_table.o \
	arch/arm32/cortex_m/irq_init.o \
	arch/arm32/cortex_m/fault.o \
	arch/arm32/cortex_m/fault_s.o \
	arch/arm32/prep_c.o \
	arch/arm32/fatal.o \
	arch/arm32/irq_manage.o \
	arch/arm32/cpu_idle.o \
	kernel/lowlevel.o \
	kernel/xip.o \
	drivers/debug/led_debug.o


