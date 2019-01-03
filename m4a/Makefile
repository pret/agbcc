AR     := ../binutils/ar
AS     := ../gas/as-new
CPP    := $(CC) -E

ifeq ($(OS),Windows_NT)
EXE := .exe
else
EXE :=
endif

CC1 := ../old_agbcc
CFLAGS := -mthumb-interwork -O2

CPPFLAGS := -I ../ginclude -I ../libc/include -nostdinc -undef
ASFLAGS := -mcpu=arm7tdmi
ARFLAGS := rc

C_SRCS := m4a_2.c m4a_4.c m4a_tables.c
C_OBJS := $(C_SRCS:%.c=%.o)

ASM_SRCS := m4a_1.s m4a_3.s
ASM_OBJS := $(ASM_SRCS:%.s=%.o)

ALL_OBJS := $(C_OBJS) $(ASM_OBJS)

LIB := libm4a.a

.PHONY: all clean

all: $(LIB)
	@:

clean:
	rm -f $(LIB) $(C_SRCS:%.c=%.i) $(C_SRCS:%.c=%.s) $(ALL_OBJS)

$(LIB): $(ALL_OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(C_OBJS): %.o: %.c
	@$(CPP) $(CPPFLAGS) $< -o $*.i
	@$(CC1) $(CFLAGS) -o $*.s $*.i
	$(AS) $(ASFLAGS) -o $@ $*.s

$(ASM_OBJS): %.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<
