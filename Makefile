COMPILER=arm-none-eabi-
CC=${COMPILER}gcc
LD=${COMPILER}ld
OBJCOPY=${COMPILER}objcopy
ARCH = arm32

TARGET = osek-os

CWD      := $(shell pwd)
INCDIRS  := -I ${CWD}/include \
            -I ${CWD}/include/arch/aarch32/ \
	    -I ${CWD}/board/*/* \
	    -I ${CWD}/include/arch/aarch32/cortex_m/ \
	    -I ${CWD}/include/arch/aarch32/cortex_m/cmsis/ \
	    -I ${CWD}/lib/include

LDFLAGS  += -nostdlib -g
CFLAGS   += -Werror ${INCDIRS} -g 
ASFLAGS  += -D__ASSEMBLY__ ${INCDIRS}

$(info compilating for "-m armelf")
#CFLAGS  += -march=armv7-m -mcpu=cortex-m4
#ASFLAGS += -march=armv7-m -mcpu=cortex-m4
#CFLAGS  += -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 
#ASFLAGS += -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 
#CFLAGS  += -mthumb -march=armv7e-m -mfloat-abi=hard -mfpu=fpv4-sp-d16 
#ASFLAGS += -mthumb -march=armv7e-m -mfloat-abi=hard -mfpu=fpv4-sp-d16 
CFLAGS  += -mthumb -march=armv7e-m -mtune=cortex-m4 
ASFLAGS += -mthumb -march=armv7e-m -mtune=cortex-m4 
LDFLAGS += -m armelf -T osex-os.lds

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

OS_OBJS	:= \
	arch/arm32/cortex_m/reset.o \
	arch/arm32/cortex_m/irq_init.o \
	arch/arm32/cortex_m/fault.o \
	arch/arm32/prep_c.o \
	arch/arm32/fatal.o \
	arch/arm32/irq_manage.o \
	arch/arm32/cpu_idle.o \
	kernel/main.o \
	kernel/xip.o

OBJS	:= $(OS_OBJS) $(LIBOBJS)


.PHONY: all

all: clean ${TARGET}

${TARGET}: ${OBJS}
	@echo Compiled for -march=armv7ve
	$(LD) $(LDFLAGS) $^ -o ${TARGET}.elf
	$(OBJCOPY) -O binary ${TARGET}.elf ${TARGET}.bin

info:
	@echo make can be run with var ARCH=${ARCH}
	@echo By default ARCH=arm64

clean:
	rm -f ${OBJS}
	rm -f ${TARGET}.bin ${TARGET}.elf
