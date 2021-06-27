CC=aarch64-none-elf-gcc
LD=aarch64-none-elf-ld
OBJCOPY=aarch64-none-elf-objcopy
ARCH = arm64
XEN_SRC = ../xen

TARGET = dom1_image

LDFLAGS  += -nostdlib
CFLAGS   += -Werror -I${XEN_SRC}/xen/include/public -D XEN_HAVE_PV_GUEST_ENTRY=1 
ASFLAGS  += -D__ASSEMBLY__ -I${XEN_SRC}/xen/include/public

$(info compilating for "-m aarch64elf")
CFLAGS  += -march=armv8-a
ASFLAGS += -march=armv8-a
LDFLAGS += -m aarch64elf -T xen-dom1.lds
OBJS    += bootstrap.aarch64.o hypercall.o


.PHONY: all

all: clean ${TARGET}

${TARGET}: $(OBJS) kernel.o
	@echo Compiled for -march=armv8-a
	$(LD) $(LDFLAGS) $^ -o ${TARGET}.elf
	$(OBJCOPY) -O binary ${TARGET}.elf ${TARGET}.bin

info:
	@echo make can be run with var ARCH=${ARCH}
	@echo By default ARCH=arm64

clean:
	rm -f *.o 
	rm -f ${TARGET}.bin ${TARGET}.elf
