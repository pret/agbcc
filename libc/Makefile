ifneq (,$(DEVKITARM))
    ifneq (,$(wildcard $(DEVKITARM)/bin))	    
        include $(DEVKITARM)/base_tools
        DKA_EXISTS=1
    else
        DKA_EXISTS=0
    endif
else
DKA_EXISTS=0
endif

PREFIX := arm-none-eabi-
export AR := $(PREFIX)ar
export AS := $(PREFIX)as

ifeq ($(DKA_EXISTS),0)
ifeq ($(shell which $(AR)),)
$(info DKA_EXISTS=$(DKA_EXISTS))
$(info DEVKITARM=$(DEVKITARM))
$(info which arm-none-eabi-ar is $(shell which arm-none-eabi-ar))
$(error Cannot find $(AR) or devkitARM, make sure you have an arm-none-eabi toolchain or devkitARM installed and your environment variables properly set up)
endif
endif

SHELL := /bin/bash -o pipefail

ASFLAGS := -mcpu=arm7tdmi

CC1    := ../build_old_agbcc/old_agbcc
CFLAGS := -O2 -fno-builtin

CPPFLAGS := -I ../ginclude -I include -nostdinc -undef \
-DABORT_PROVIDED -DHAVE_GETTIMEOFDAY -D__thumb__ -DARM_RDI_MONITOR \
-D__GNUC__ -DINTERNAL_NEWLIB -D__USER_LABEL_PREFIX__=


# Clear the default suffixes.
.SUFFIXES:

# Secondary expansion is required for dependency variables in object rules.
.SECONDEXPANSION:

.PHONY: all clean

C_SRCS := $(shell find . -iname "*.c" -a ! -iname "mallocr.c")
C_INTRS := $(C_SRCS:%.c=%.i)
C_OBJS := $(C_SRCS:%.c=%.o) \
stdlib/mallocr.o stdlib/freer.o stdlib/reallocr.o stdlib/callocr.o \
stdlib/cfreer.o stdlib/malignr.o stdlib/vallocr.o stdlib/pvallocr.o \
stdlib/mallinfor.o stdlib/mallstatsr.o stdlib/msizer.o stdlib/malloptr.o \
stdio/vfiprintf.o

S_SRCS := arm/setjmp.s arm/trap.s
S_OBJS := $(S_SRCS:%.s=%.o)

OBJS := $(C_OBJS) $(S_OBJS)

all: libc.a
	@:

clean:
	rm -f $(OBJS)
	rm -f $(C_OBJS:%.o=%.i)
	rm -f $(C_OBJS:%.o=%.s)
	rm -f $(S_OBJS:%.o=%.i)
	rm -f libc.a

PREPROCESS := $(CPP) $(CPPFLAGS)

stdlib/mallocr.i: stdlib/mallocr.c
	$(PREPROCESS) -DDEFINE_MALLOC $< -o $@

stdlib/freer.i: stdlib/mallocr.c
	$(PREPROCESS) -DDEFINE_FREE $< -o $@

stdlib/reallocr.i: stdlib/mallocr.c
	$(PREPROCESS) -DDEFINE_REALLOC $< -o $@

stdlib/callocr.i: stdlib/mallocr.c
	$(PREPROCESS) -DDEFINE_CALLOC $< -o $@

stdlib/cfreer.i: stdlib/mallocr.c
	$(PREPROCESS) -DDEFINE_CFREE $< -o $@

stdlib/malignr.i: stdlib/mallocr.c
	$(PREPROCESS) -DDEFINE_MEMALIGN $< -o $@

stdlib/vallocr.i: stdlib/mallocr.c
	$(PREPROCESS) -DDEFINE_VALLOC $< -o $@

stdlib/pvallocr.i: stdlib/mallocr.c
	$(PREPROCESS) -DDEFINE_PVALLOC $< -o $@

stdlib/mallinfor.i: stdlib/mallocr.c
	$(PREPROCESS) -DDEFINE_MALLINFO $< -o $@

stdlib/mallstatsr.i: stdlib/mallocr.c
	$(PREPROCESS) -DDEFINE_MALLOC_STATS $< -o $@

stdlib/msizer.i: stdlib/mallocr.c
	$(PREPROCESS) -DDEFINE_MALLOC_USABLE_SIZE $< -o $@

stdlib/malloptr.i: stdlib/mallocr.c
	$(PREPROCESS) -DDEFINE_MALLOPT $< -o $@

stdio/vfiprintf.i: stdio/vfprintf.c
	$(PREPROCESS) -DINTEGER_ONLY $< -o $@

$(C_INTRS): %.i : %.c
	$(PREPROCESS) $< -o $@

stdlib/mbtowc_r.o: CFLAGS := $(CFLAGS) -fshort-enums

$(C_OBJS): %.o : %.i
	@$(CC1) $(CFLAGS) $< -o $*.s
	@printf ".text\n\t.align\t2, 0\n" >> $*.s
	$(AS) $(ASFLAGS) -o $@ $*.s

$(S_OBJS): %.o: %.s
	@$(CPP) $(CPPFLAGS) $< -o $*.i
	@printf ".text\n\t.align\t2, 0\n" >> $*.i
	$(AS) $(ASFLAGS) -o $@ $*.i

libc.a: $(OBJS)
	$(AR) -rc libc.a $(OBJS)
