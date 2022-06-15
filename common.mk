TARGET = osek-os

INCDIRS  += -I ${CWD}/board

CMN_OBJS := \
	${CWD}/kernel/os_entry.o \
	${CWD}/kernel/os_log.o \
	${CWD}/kernel/os_fifo.o \
	${CWD}/kernel/os_task.o \
	${CWD}/kernel/os_event.o \
	${CWD}/kernel/os_alarm.o \
	${CWD}/kernel/os_isr.o \
	${CWD}/kernel/os_res.o


$(info compiling Common source files)
ifeq ($(OS),Windows_NT)
	MINGW_BUILD = TRUE
else
	MINGW_BUILD = FALSE
endif	 

OBJS	:= $(CMN_OBJS) $(BRD_OBJS) $(ARCH_OBJS) $(LIBOBJS) $(SG_OBJS) $(APP_OBJS)

.PHONY: all
.DEFAULT_GOAL := all

build_check:
	@if [ ${MINGW_BUILD} = TRUE ]; then\
		if [ -z "${MINGW_ROOT}" ]; then\
			printf "\nError: MINGW_ROOT is not defined! Please define it as a environmental variable!";\
			printf "\n[Hint: MINGW_ROOT should point to MSYS2 installation path (D:\msys64\mingw64)]\n";\
			exit 1;\
		fi;\
	fi

${TARGET}: ${OBJS}
	@echo LINKING OBJECTS...
	$(LD) $^ -o ${TARGET}.elf $(LDFLAGS) -Map=${TARGET}.map
	$(OBJCOPY) -O binary ${TARGET}.elf ${TARGET}.bin


all: build_check ${TARGET}


info:
	@echo make can be run with var ARCH=${ARCH}
	@echo By default ARCH=arm64


clean:
	rm -f ${OBJS}
	rm -f ${TARGET}.bin ${TARGET}.elf ${TARGET}.map