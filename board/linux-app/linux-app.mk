COMPILER=
CC=${COMPILER}gcc
LD=${COMPILER}gcc
AS=${COMPILER}as
OBJCOPY=${COMPILER}objcopy
ARCH = x86

INCDIRS  := -I ${CWD}/include \
	    -I ${CWD}/board/linux-app 

LDFLAGS  += -g 
CFLAGS   += -Werror ${INCDIRS} -g
ASFLAGS  += -D__ASSEMBLY__ ${INCDIRS} -g

$(info compilating for "Linux PC")


BRD_OBJS	:= \
	${CWD}/board/linux-app/board.o \
	${CWD}/kernel/kernel.o

