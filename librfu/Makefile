AR     := ../binutils/ar
AS     := ../gas/as-new
CPP    := $(CC) -E

ifeq ($(OS),Windows_NT)
EXE := .exe
else
EXE :=
endif

RFU_VERSION ?= 1024

CC1 := ../agbcc
CFLAGS := -mthumb-interwork -O2

CPPFLAGS := -I ../ginclude -I ../libc/include -nostdinc -undef
ASFLAGS := -mcpu=arm7tdmi --defsym RFU_VERSION=$(RFU_VERSION)
ARFLAGS := -rc

C_SRCS := librfu_rfu.c librfu_stwi.c
C_OBJS := $(C_SRCS:%.c=%_$(RFU_VERSION).o)

ASM_SRCS := librfu_intr.s librfu.s
ASM_OBJS := $(ASM_SRCS:%.s=%_$(RFU_VERSION).o)

ALL_OBJS := $(C_OBJS) $(ASM_OBJS)

LIB := librfu_$(RFU_VERSION).a

.PHONY: all clean

all: $(LIB)
	@:

clean:
	rm -f *.a *.o $(C_SRCS:%.c=%.i) $(C_SRCS:%.c=%.s)

$(LIB): %.a: $(ALL_OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(C_OBJS): %_$(RFU_VERSION).o: %.c
	@$(CPP) $(CPPFLAGS) $< -o $*.i
	@$(CC1) $(CFLAGS) -o $*.s $*.i
	$(AS) $(ASFLAGS) -o $@ $*.s

$(ASM_OBJS): %_$(RFU_VERSION).o: %.s
	$(AS) $(ASFLAGS) -o $@ $<

v1024: ; @$(MAKE) RFU_VERSION=1024
v1026: ; @$(MAKE) RFU_VERSION=1026
