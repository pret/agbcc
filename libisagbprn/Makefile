AR     := ../binutils/ar
AS     := ../gas/as-new
CPP    := $(CC) -E

ifeq ($(OS),Windows_NT)
EXE := .exe
else
EXE :=
endif

CC1 := ../agbcc
CFLAGS := -O2 -mthumb-interwork -fhex-asm

CPPFLAGS := -I ../ginclude -I ../libc/include -nostdinc -undef
ASFLAGS := -mcpu=arm7tdmi
ARFLAGS := rc

C_SRCS := libisagbprn.c nocashgba.c
C_OBJS := $(C_SRCS:%.c=%.o)

LIB := libisagbprn.a

.PHONY: all clean

all: $(LIB)
	@:

clean:
	rm -f $(LIB) $(C_SRCS:%.c=%.i) $(C_SRCS:%.c=%.s) $(C_OBJS)

$(LIB): $(C_OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(C_OBJS): %.o: %.c
	@$(CPP) $(CPPFLAGS) $< -o $*.i
	@$(CC1) $(CFLAGS) -o $*.s $*.i
	$(AS) $(ASFLAGS) -o $@ $*.s
