CC=${COMPILER}gcc
LD=${COMPILER}ld
AS=${COMPILER}as
AR=${COMPILER}ar
RANLIB=${COMPILER}ranlib
OBJCOPY=${COMPILER}objcopy

include ${ROOT_DIR}/path_defs.mk

INCDIRS  += 	-I ${OS_PATH}/include \
		-I ${OS_PATH}/lib/include \
		-I ${MCU_BOARD_PATH} \
		-I ${MCU_MICRO_PATH} \
		-I ${MCU_PATH}/src \
		-I ${MCU_PATH}/src/common \
		-I ${MCU_PATH}/src/common/src \
		-I ${MCU_PATH}/src/common/api \
		-I ${MCU_MICRO_PATH} \
		-I ${OS_BUILDER_PATH}/src

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

#	${OS_PATH}/lib/libc-minimal/stdout/fprintf.o \
#	${OS_PATH}/lib/libc-minimal/stdlib/malloc.o \


LIBOBJS	:= \
	${OS_PATH}/lib/libc-minimal/string/string.o \
	${OS_PATH}/lib/libc-minimal/stdout/printf.o


LDFLAGS := -g
CFLAGS  := -Werror ${INCDIRS} -g
ASFLAGS := ${INCDIRS} -g
TARGET  := libOs.la
# include c_l_flags.mk to add more definitions specific to micro-controller
include ${ROOT_DIR}/c_l_flags.mk

all: $(TARGET)

LIB_OBJS := $(CMN_OBJS) $(STDLIBOBJS) $(LIBOBJS)

$(TARGET): $(LIB_OBJS)
	$(AR) r $@ $^
	$(RANLIB) $@

clean:
	$(RM) $(LIB_OBJS) $(TARGET)