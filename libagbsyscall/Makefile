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

SYSCALLS := ArcTan2 \
            BgAffineSet \
            CpuFastSet \
            CpuSet \
            Div \
            LZ77UnCompVram \
            LZ77UnCompWram \
            MultiBoot \
            ObjAffineSet \
            RLUnCompVram \
            RLUnCompWram \
            RegisterRamReset \
            SoftReset \
            Sqrt \
            VBlankIntrWait

ASM_SRCS := libagbsyscall.s
ASM_OBJS := $(foreach syscall, $(SYSCALLS), $(syscall).o)

LIB := libagbsyscall.a

.PHONY: all clean $(LIB)

all: $(LIB)

clean:
	rm -f $(LIB) $(ASM_OBJS)

$(LIB): $(ASM_OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(ASM_OBJS): libagbsyscall.s
	$(AS) $(ASFLAGS) --defsym L_$(@:%.o=%)=1 -o $@ $<
