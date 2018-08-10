CC1 = ../old_agbcc
AR = ../binutils/binutils/ar
AS = ../binutils/gas/as-new
CPP = cpp
CPPFLAGS := -undef -nostdinc

LIB1ASMFUNCS = _udivsi3 _divsi3 _umodsi3 _modsi3 _dvmd_tls _call_via_rX
LIB1ASMOBJS = $(addsuffix .o,$(LIB1ASMFUNCS))

LIB2FUNCS = _muldi3 _divdi3 _moddi3 _udivdi3 _umoddi3 _negdi2 \
_lshrdi3 _ashldi3 _ashrdi3 _ffsdi2 \
_udiv_w_sdiv _udivmoddi4 _cmpdi2 _ucmpdi2 _floatdidf _floatdisf \
_fixunsdfsi _fixunssfsi _fixunsdfdi _fixdfdi _fixunssfdi _fixsfdi
LIB2OBJS = $(addsuffix .o,$(LIB2FUNCS))

libgcc.a: $(LIB1ASMOBJS) $(LIB2OBJS) fp-bit.o dp-bit.o
	$(AR) -rc $@ $?

$(LIB1ASMOBJS): lib1thumb.asm
	$(CPP) -P $(CPPFLAGS) -DL$(basename $@) -x assembler-with-cpp -o $(basename $@).s lib1thumb.asm
	@printf ".text\n\t.align\t2, 0\n" >> $(basename $@).s
	$(AS) -mcpu=arm7tdmi -o $@ $(basename $@).s

$(LIB2OBJS): libgcc2.c longlong.h
	$(CPP) $(CPPFLAGS) -I ../ginclude -DL$(basename $@) libgcc2.c | $(CC1) -O2 -o $(basename $@).s
	@printf ".text\n\t.align\t2, 0\n" >> $(basename $@).s
	$(AS) -mcpu=arm7tdmi -o $@ $(basename $@).s

fp-bit.o: CPPFLAGS += -DFLOAT -DFLOAT_BIT_ORDER_MISMATCH
dp-bit.o: CPPFLAGS += -DFLOAT_BIT_ORDER_MISMATCH -DFLOAT_WORD_ORDER_MISMATCH

fp-bit.o: %.o: fp-bit-base.c
	$(CPP) -P $(CPPFLAGS) -I ../ginclude $< | $(CC1) -O2 -o $*.s
	@printf ".text\n\t.align\t2, 0\n" >> $*.s
	@printf ".text\n\t.align\t2, 0\n" >> $*.s
	$(AS) -mcpu=arm7tdmi -o $@ $*.s

dp-bit.o: %.o: fp-bit-base.c
	$(CPP) -P $(CPPFLAGS) -I ../ginclude $< | $(CC1) -O2 -o $*.s
	@printf ".text\n\t.align\t2, 0\n" >> $*.s
	$(AS) -mcpu=arm7tdmi -o $@ $*.s

.PHONY: clean

clean:
	rm -f *.o *.a *.s *.i
