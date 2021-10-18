COMPILER=arm-none-eabi-
CC=${COMPILER}gcc
LD=${COMPILER}ld
AS=${COMPILER}as
OBJCOPY=${COMPILER}objcopy
ARCH = arm32
BOARD_NAME=qemu-versatilepb

INCDIRS  := -I ${CWD}/include \
            -I ${CWD}/include/arch/aarch32/ \
	    -I ${CWD}/board/${BOARD_NAME} \
	    -I ${CWD}/include/arch/aarch32/cortex_m/ \
	    -I ${CWD}/include/arch/aarch32/cortex_m/cmsis/ \
	    -I ${CWD}/lib/include

LDFLAGS  += -nostdlib -g
CFLAGS   += -Werror ${INCDIRS} -g
ASFLAGS  += -D__ASSEMBLY__ ${INCDIRS} -g

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
	lib/libc-minimal/stdlib/malloc.o \
	lib/libc-minimal/stdlib/strtol.o \
	lib/libc-minimal/stdlib/strtoul.o \

LIBOBJS	:= \
	lib/libc-minimal/string/string.o

BRD_OBJS	:= \
	${CWD}/board/qemu-versatilepb/board.o \
	${CWD}/board/qemu-versatilepb/startup.o

BRD_OBJS_ASM    := \
	${CWD}/board/qemu-versatilepb/startup.o

