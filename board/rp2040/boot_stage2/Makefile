COMPILER=arm-none-eabi-
CC=${COMPILER}gcc
LD=${COMPILER}ld
AS=${COMPILER}as
OBJCOPY=${COMPILER}objcopy

TARGET = boot_stage2
CWD=$(shell pwd)

LDFILE	:= ${CWD}/boot_stage2.lds
LDFLAGS  += -nostdlib -g -m armelf -T ${LDFILE}

INCDIRS  += -I ${CWD}/include/boot_stage2 -I ${CWD}/asminclude/boot2_helpers \
	-I ${CWD}/asminclude
	
CFLAGS := ${INCDIRS} 

BTL2_OBJS := \
	${CWD}/boot2_w25q080.o


$(info compiling Common source files)
ifeq ($(OS),Windows_NT)
	MINGW_BUILD = TRUE
else
	MINGW_BUILD = FALSE
endif	 

OBJS	:= $(BTL2_OBJS)

.PHONY: all
.DEFAULT_GOAL := all

build_check:
	@if [ ${MINGW_BUILD} = TRUE ]; then\
		if [ -z "${MINGW_ROOT}" ]; then\
			printf "\nError: MINGW_ROOT is not defined! Please define it as a environmental variable!";\
			printf "\n[Hint: MINGW_ROOT should point to MSYS2 installation path (D:\msys64\mingw64)]\n";\
			exit 1;\
		fi;\
	fi
	
%.o : %.S
	$(CC) $(CFLAGS) $< -c

${TARGET}: ${OBJS}
	@echo LINKING OBJECTS...
	$(LD) $^ -o ${TARGET}.elf $(LDFLAGS) -Map=${TARGET}.map
	$(OBJCOPY) -O binary ${TARGET}.elf ${TARGET}.bin
	python3 ${CWD}/pad_checksum -s 0xffffffff ${TARGET}.bin ../${TARGET}.s


all: build_check ${TARGET}

info:
	@echo make can be run with var ARCH=${ARCH}
	@echo By default ARCH=arm64

clean:
	rm -f ${OBJS}
	rm -f ${TARGET}.bin ${TARGET}.elf ${TARGET}.map