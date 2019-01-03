AR     := ../binutils/ar
AS     := ../gas/as-new
CPP    := $(CC) -E

ifeq ($(OS),Windows_NT)
EXE := .exe
else
EXE :=
endif

CC1 := ../agbcc
CFLAGS := -O2 -mthumb-interwork

CPPFLAGS := -I ../ginclude -I ../libc/include -nostdinc -undef
ASFLAGS := -mcpu=arm7tdmi
ARFLAGS := rc

C_SRCS := agb_flash.c agb_flash_1m.c agb_flash_le.c agb_flash_mx.c
C_OBJS := $(C_SRCS:%.c=%.o)

agb_flash.o: CFLAGS := -O -mthumb-interwork
agb_flash_1m.o: CFLAGS := -O -mthumb-interwork
agb_flash_mx.o: CFLAGS := -O -mthumb-interwork

LIB := libagb_flash.a

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
