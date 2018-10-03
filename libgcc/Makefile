# Prefer the faster mksh over Bash, we just need pipefail
ifneq (,$(shell command -v mksh || true))
SHELL := mksh -o pipefail
# Android's /system/bin/sh is also mksh
else ifneq (,$(wildcard /system/bin/sh))
SHELL := /system/bin/sh -o pipefail
# otherwise, use Bash.
else ifneq (,$(shell command -v bash || true))
SHELL := bash -o pipefail
endif

CC1 := ../old_agbcc
AR := ../binutils/ar
AS := ../gas/as-new
CPP := $(CC) -E
CPPFLAGS := -undef -nostdinc

LIB1ASMFUNCS := _udivsi3 _divsi3 _umodsi3 _modsi3 _dvmd_tls _call_via_rX
LIB1ASMOBJS := $(addsuffix .o,$(LIB1ASMFUNCS))

LIB2FUNCS := _muldi3 _divdi3 _moddi3 _udivdi3 _umoddi3 _negdi2 \
_lshrdi3 _ashldi3 _ashrdi3 _ffsdi2 \
_udiv_w_sdiv _udivmoddi4 _cmpdi2 _ucmpdi2 _floatdidf _floatdisf \
_fixunsdfsi _fixunssfsi _fixunsdfdi _fixdfdi _fixunssfdi _fixsfdi
LIB2OBJS := $(addsuffix .o,$(LIB2FUNCS))

libgcc.a: $(LIB1ASMOBJS) $(LIB2OBJS) fp-bit.o dp-bit.o
	$(AR) -rc $@ $?

$(LIB1ASMOBJS): lib1thumb.asm
	$(CPP) -P $(CPPFLAGS) -DL$(basename $@) -x assembler-with-cpp lib1thumb.asm | $(AS) -mcpu=arm7tdmi -o $@

$(LIB2OBJS): libgcc2.c longlong.h
	$(CPP) $(CPPFLAGS) -I ../ginclude -DL$(basename $@) libgcc2.c | $(CC1) -O2 | $(AS) -mcpu=arm7tdmi -o $@

fp-bit.o: CPPFLAGS += -DFLOAT -DFLOAT_BIT_ORDER_MISMATCH
dp-bit.o: CPPFLAGS += -DFLOAT_BIT_ORDER_MISMATCH -DFLOAT_WORD_ORDER_MISMATCH

fp-bit.o: %.o: fp-bit-base.c
	$(CPP) -P $(CPPFLAGS) -I ../ginclude $< | $(CC1) -O2 | $(AS) -mcpu=arm7tdmi -o $@

dp-bit.o: %.o: fp-bit-base.c
	$(CPP) -P $(CPPFLAGS) -I ../ginclude $< | $(CC1) -O2 | $(AS) -mcpu=arm7tdmi -o $@

.PHONY: clean

clean:
	rm -f *.o *.a *.s *.i
