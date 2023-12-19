CC=${COMPILER}gcc
LD=${COMPILER}ld
AS=${COMPILER}as
AR=${COMPILER}ar
RANLIB=${COMPILER}ranlib
OBJCOPY=${COMPILER}objcopy

# CAR_OS_PATH will be passed as argument from main Makefile in Car-OS directory
include ${CAR_OS_PATH}/path_defs.mk
include ${OS_BUILDER_PATH}/src/os-objs.mk


INCDIRS  += 	-I ${OS_PATH}/include \
		-I ${OS_PATH}/lib/include \
		-I ${CAR_OS_INC_PATH}/autosar \
		-I ${CAR_OS_INC_PATH}/car_os \
		-I ${CAR_OS_BOARDS_PATH} \
		-I ${MCU_PATH}/src \
		-I ${MCU_PATH}/src/common \
		-I ${MCU_PATH}/src/common/src \
		-I ${MCU_PATH}/src/common/api \
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
	${OS_PATH}/lib/libc-minimal/stdlib/error.o \
	${OS_PATH}/lib/libc-minimal/stdlib/exit.o \
	${OS_PATH}/lib/libc-minimal/stdlib/strtol.o \
	${OS_PATH}/lib/libc-minimal/stdlib/strtoul.o \

#	${OS_PATH}/lib/libc-minimal/stdout/fprintf.o \
#	${OS_PATH}/lib/libc-minimal/stdlib/malloc.o \


OS_LIBOBJS	:= \
	${OS_PATH}/lib/libc-minimal/string/string.o \
	${OS_PATH}/lib/libc-minimal/stdout/printf.o



$(info  )
$(info compiling Os source files)


LDFLAGS := -g -relocatable
CFLAGS  := -Werror ${INCDIRS} -g
ASFLAGS := ${INCDIRS} -g
TARGET  := libOs.la
# include c_l_flags.mk to add more definitions specific to micro-controller
include ${CAR_OS_PATH}/c_l_flags.mk

all: $(TARGET)

LIB_OBJS := $(CMN_OBJS) $(STDLIBOBJS) $(OS_LIBOBJS) $(SG_OBJS)

$(TARGET): $(LIB_OBJS)
	$(LD) ${LDFLAGS} -o $@ $^

clean:
	$(RM) $(LIB_OBJS) $(TARGET)
