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
		-I ${CAR_OS_BOARDSOC_PATH} \
		-I ${ZEPHYR_INC_Z_PATH} \
		-I ${OS_BUILDER_PATH}/src


CMN_OBJS := \
	${OS_PATH}/kernel/os_entry.o \
	${OS_PATH}/kernel/os_task.o \
	${OS_PATH}/kernel/os_event.o \
	${OS_PATH}/kernel/os_counter.o \
	${OS_PATH}/kernel/os_alarm.o \
	${OS_PATH}/kernel/os_isr.o \
	${OS_PATH}/kernel/os_res.o



$(info  )
$(info compiling Os source files)


TARGET  := libOs.a

# include c_l_flags.mk to add more definitions specific to micro-controller
include ${CAR_OS_PATH}/c_l_flags.mk



%.o: %.c
	$(CC) -c ${CFLAGS} ${INCDIRS} $< -o $@


all: $(TARGET)

# LIB_OBJS := $(CMN_OBJS) $(STDLIBOBJS) $(OS_LIBOBJS) $(SG_OBJS)
LIB_OBJS := $(CMN_OBJS) $(SG_OBJS)


$(TARGET): $(LIB_OBJS)
	$(AR) -rcs ${TARGET} ${LIB_OBJS}

# $(LD) ${LDFLAGS} -o $@ $^

clean:
	$(RM) $(LIB_OBJS) $(TARGET)
