COMPILER=
CC=${COMPILER}gcc
LD=${COMPILER}gcc
AS=${COMPILER}as
OBJCOPY=${COMPILER}objcopy
ARCH = x86

INCDIRS  += -I ${CWD}/test

LDFLAGS  += -g
CFLAGS   += -Werror ${INCDIRS} -g
ASFLAGS  += -D__ASSEMBLY__ ${INCDIRS} -g

$(info compilating System Generator source files)


APP_OBJS	:= \
	${CWD}/test/test_app.o

