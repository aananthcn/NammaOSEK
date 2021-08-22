TARGET = osek-os


CMN_OBJS := \
	${CWD}/kernel/os_entry.o \
	${CWD}/kernel/os_fifo.o \
	${CWD}/kernel/os_task.o \
	${CWD}/kernel/os_log.o \
	${CWD}/kernel/os_alarm.o



OBJS	:= $(CMN_OBJS) $(BRD_OBJS) $(LIBOBJS) $(SG_OBJS) $(APP_OBJS)

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