COMPILER=arm-none-eabi-
CC=${COMPILER}gcc
LD=${COMPILER}ld
AS=${COMPILER}as
OBJCOPY=${COMPILER}objcopy
ARCH = arm32
BOARD_NAME=stm32f407vet6

CC_VERS := $(shell ${CC} -dumpfullversion)
LIB_GCC_A_PATH=/usr/lib/gcc/arm-none-eabi/${CC_VERS}

INCDIRS  := -I ${CWD}/include \
            -I ${CWD}/include/arch/aarch32/ \
	    -I ${CWD}/board/${BOARD_NAME} \
	    -I ${CWD}/include/arch/aarch32/cortex_m/ \
	    -I ${CWD}/include/arch/aarch32/cortex_m/cmsis/ \
	    -I ${CWD}/lib/include

LDFLAGS  += -nostdlib -g -L${LIB_GCC_A_PATH} -lgcc
CFLAGS   += -Werror ${INCDIRS} -g
ASFLAGS  += ${INCDIRS} -g

$(info compilating stm32f407vet6 board specific files)
#CFLAGS  += -march=armv7e-m 
#ASFLAGS += -march=armv7e-m
CFLAGS  += -mthumb -mthumb-interwork -march=armv7e-m -mcpu=cortex-m4 
ASFLAGS += -mthumb -mthumb-interwork -march=armv7e-m -mcpu=cortex-m4 
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
	${CWD}/board/stm32f407vet6/board.o \
	${CWD}/board/stm32f407vet6/vector_handlers.o \
	${CWD}/board/stm32f407vet6/vectors.o \
	${CWD}/board/stm32f407vet6/startup.o

