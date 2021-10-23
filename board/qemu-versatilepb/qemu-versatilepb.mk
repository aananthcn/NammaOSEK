COMPILER=arm-none-eabi-
CC=${COMPILER}gcc
LD=${COMPILER}ld
AS=${COMPILER}as
OBJCOPY=${COMPILER}objcopy
ARCH = arm32
BOARD_NAME=qemu-versatilepb

# The following path is available in ${TOOLCHAIN_PATH} env variable, but I can't get it...
LIB_GCC_A_PATH=/opt/tools/cortex-m/gcc-arm-none-eabi-10-2020-q4-major/lib/gcc/arm-none-eabi/10.2.1

INCDIRS  := -I ${CWD}/include \
            -I ${CWD}/include/arch/aarch32/ \
	    -I ${CWD}/board/${BOARD_NAME} \
	    -I ${CWD}/include/arch/aarch32/cortex_m/ \
	    -I ${CWD}/include/arch/aarch32/cortex_m/cmsis/ \
	    -I ${CWD}/lib/include

LDFLAGS  += -nostdlib -g -L${LIB_GCC_A_PATH} -lgcc
CFLAGS   += -Werror ${INCDIRS} -g
ASFLAGS  += ${INCDIRS} -g

$(info compilating qemu-versatilepb board specific files)
CFLAGS  += -mcpu=arm926ej-s 
ASFLAGS += -mcpu=arm926ej-s 
LDFILE	:= ${CWD}/board/${BOARD_NAME}/${BOARD_NAME}.lds
LDFLAGS += -m armelf -T ${LDFILE}


STDLIBOBJS	:= \
	lib/libc-minimal/stdlib/abort.o \
	lib/libc-minimal/stdlib/atoi.o \
	lib/libc-minimal/stdlib/bsearch.o \
	lib/libc-minimal/stdlib/exit.o \
	lib/libc-minimal/stdlib/strtol.o \
	lib/libc-minimal/stdlib/strtoul.o \
	lib/libc-minimal/stdout/fprintf.o \

#	lib/libc-minimal/stdlib/malloc.o \


LIBOBJS	:= \
	lib/libc-minimal/string/string.o \
	lib/libc-minimal/stdout/printf.o
	

BRD_OBJS	:= \
	${CWD}/board/qemu-versatilepb/board.o \
	${CWD}/board/qemu-versatilepb/vector_handlers.o \
	${CWD}/board/qemu-versatilepb/vectors.o \
	${CWD}/board/qemu-versatilepb/startup.o

