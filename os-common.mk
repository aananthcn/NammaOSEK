TARGET = NammaAutosar

INCDIRS  += 	-I ${OS_PATH}/board \
		-I ${OS_PATH}/include \
		-I ${OS_PATH}/lib/include \
		-I ${OS_PATH}/tools/src \
		-I ${MCU_BOARD_PATH} \
		-I ${MCU_MICRO_PATH}

CMN_OBJS := \
	${OS_PATH}/kernel/os_entry.o \
	${OS_PATH}/kernel/os_log.o \
	${OS_PATH}/kernel/os_fifo.o \
	${OS_PATH}/kernel/os_task.o \
	${OS_PATH}/kernel/os_event.o \
	${OS_PATH}/kernel/os_alarm.o \
	${OS_PATH}/kernel/os_isr.o \
	${OS_PATH}/kernel/os_res.o


STDLIBOBJS	:= \
	${OS_PATH}/lib/libc-minimal/stdlib/abort.o \
	${OS_PATH}/lib/libc-minimal/stdlib/atoi.o \
	${OS_PATH}/lib/libc-minimal/stdlib/bsearch.o \
	${OS_PATH}/lib/libc-minimal/stdlib/exit.o \
	${OS_PATH}/lib/libc-minimal/stdlib/strtol.o \
	${OS_PATH}/lib/libc-minimal/stdlib/strtoul.o \
	${OS_PATH}/lib/libc-minimal/stdout/fprintf.o \

#	${OS_PATH}/lib/libc-minimal/stdlib/malloc.o \


LIBOBJS	:= \
	${OS_PATH}/lib/libc-minimal/string/string.o \
	${OS_PATH}/lib/libc-minimal/stdout/printf.o


$(info compiling AUTOSAR Os Common source files)
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