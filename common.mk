TARGET = osek-os

OBJS	:= $(BRD_OBJS) $(LIBOBJS) $(SG_OBJS)


.PHONY: all

all: ${TARGET}

${TARGET}: ${OBJS}
	@echo Compiled for -march=${ARCH}
	$(LD) $^ -o ${TARGET}.elf $(LDFLAGS) 
	$(OBJCOPY) -O binary ${TARGET}.elf ${TARGET}.bin

info:
	@echo make can be run with var ARCH=${ARCH}
	@echo By default ARCH=arm64

clean:
	rm -f ${OBJS}
	rm -f ${TARGET}.bin ${TARGET}.elf