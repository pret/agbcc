include $(DEVKITARM)/base_tools
export AR := $(PREFIX)ar
export AS := $(PREFIX)as

ifeq ($(OS),Windows_NT)
EXE := .exe
else
EXE :=
endif

ASFLAGS := -mcpu=arm7tdmi
ARFLAGS := rc

ASM_SRCS := libgcnmultiboot.s
ASM_OBJS := $(ASM_SRCS:%.s=%.o)

LIB := libgcnmultiboot.a

.PHONY: all clean $(LIB)

all: $(LIB)

clean:
	rm -f $(LIB) $(ASM_OBJS)

$(LIB): $(ASM_OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(ASM_OBJS): %.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<
