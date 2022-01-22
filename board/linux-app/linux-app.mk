COMPILER=
CC=${COMPILER}gcc
LD=${COMPILER}gcc
AS=${COMPILER}as
OBJCOPY=${COMPILER}objcopy
ARCH = x86

INCDIRS  := -I ${CWD}/include \
	    -I ${CWD}/board/linux-app 

LDFLAGS  += -g -lrt
CFLAGS   += -Werror ${INCDIRS} -g
ASFLAGS  += ${INCDIRS} -g

$(info compiling for "Linux PC")


BRD_OBJS	:= \
	${CWD}/board/linux-app/board.o \
	${CWD}/board/linux-app/brd_timer.o

