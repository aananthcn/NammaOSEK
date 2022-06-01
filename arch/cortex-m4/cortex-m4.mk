
INCDIRS  += -I ${CWD}/arch/arm

LDFLAGS  += -g
CFLAGS   += -Werror ${INCDIRS} -g
ASFLAGS  += ${INCDIRS} -g

$(info compiling arch/arm source files)


ARCH_OBJS	:= \
	${CWD}/arch/cortex-m4/lowlevel.o \
	${CWD}/arch/cortex-m4/interrupt.o

