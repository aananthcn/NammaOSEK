TARGET = NammaAutosar

INCDIRS  += 	-I ${OS_PATH}/include \
		-I ${OS_PATH}/lib/include \
		-I ${MCU_STARTUP_PATH} \
		-I ${MCU_PATH}/src/common/src \
		-I ${MCU_PATH}/src/common/api

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

