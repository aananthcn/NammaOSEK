TARGET = osek-os


CMN_OBJS := \
	${CWD}/kernel/os_entry.o \
	${CWD}/kernel/os_log.o \
	${CWD}/kernel/os_fifo.o \
	${CWD}/kernel/os_task.o \
	${CWD}/kernel/os_event.o \
	${CWD}/kernel/os_alarm.o


$(info compilating Common source files)

OBJS	:= $(CMN_OBJS) $(BRD_OBJS) $(LIBOBJS) $(SG_OBJS) $(APP_OBJS)

.PHONY: all

${TARGET}: ${OBJS}
	@echo LINKING OBJECTS...
	$(LD) $^ -o ${TARGET}.elf $(LDFLAGS) -Map=${TARGET}.map
	$(OBJCOPY) -O binary ${TARGET}.elf ${TARGET}.bin

all: ${TARGET}

info:
	@echo make can be run with var ARCH=${ARCH}
	@echo By default ARCH=arm64

clean:
	rm -f ${OBJS}
	rm -f ${TARGET}.bin ${TARGET}.elf