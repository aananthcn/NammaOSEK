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
	    -I ${CWD}/include/arch/aarch32/cortex_m/

LDFLAGS  += -nostdlib
CFLAGS   += -Werror ${INCDIRS} 
ASFLAGS  += -D__ASSEMBLY__ ${INCDIRS}

$(info compilating for "-m armelf")
#CFLAGS  += -march=armv7-m -mcpu=cortex-m4
#ASFLAGS += -march=armv7-m -mcpu=cortex-m4
CFLAGS  += -march=armv7-m 
ASFLAGS += -march=armv7-m
LDFLAGS += -m armelf -T osex-os.lds
OBJS    := \
	arch/arm32/cortex_m/reset.o \
	arch/arm32/prep_c.o


.PHONY: all

all: clean ${TARGET}

${TARGET}: $(OBJS)
	@echo Compiled for -march=armv7ve
	$(LD) $(LDFLAGS) $^ -o ${TARGET}.elf
	$(OBJCOPY) -O binary ${TARGET}.elf ${TARGET}.bin

info:
	@echo make can be run with var ARCH=${ARCH}
	@echo By default ARCH=arm64

clean:
	rm -f *.o 
	rm -f ${TARGET}.bin ${TARGET}.elf
