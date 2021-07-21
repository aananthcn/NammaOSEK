COMPILER=
CC=${COMPILER}gcc
LD=${COMPILER}gcc
AS=${COMPILER}as
OBJCOPY=${COMPILER}objcopy
ARCH = x86

INCDIRS  += -I ${CWD}/tools/src 

LDFLAGS  += -g
CFLAGS   += -Werror ${INCDIRS} -g
ASFLAGS  += -D__ASSEMBLY__ ${INCDIRS} -g

$(info compilating System Generator source files)


SG_OBJS	:= \
	${CWD}/tools/src/sg_counter.o \
	${CWD}/tools/src/sg_tasks.o

