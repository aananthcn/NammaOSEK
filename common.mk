TARGET = osek-os

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

	
OBJS	:= $(BRD_OBJS) $(LIBOBJS)


.PHONY: all

all: ${TARGET}

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