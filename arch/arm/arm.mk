
INCDIRS  += -I ${CWD}/arch/arm

LDFLAGS  += -g
CFLAGS   += -Werror ${INCDIRS} -g
ASFLAGS  += ${INCDIRS} -g

$(info compilating arch/arm source files)


ARCH_OBJS	:= \
	${CWD}/arch/arm/stack.o \
	${CWD}/arch/arm/interrupt.o

