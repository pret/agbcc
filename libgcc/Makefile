CC1 = ../old_agbcc
CPP = $(DEVKITARM)/bin/arm-none-eabi-cpp
AS = $(DEVKITARM)/bin/arm-none-eabi-as
AR = $(DEVKITARM)/bin/arm-none-eabi-ar

libgcc.a: libgcc1.a libgcc2.a fp-bit.o dp-bit.o
	$(AR) -x libgcc1.a
	$(AR) -x libgcc2.a
	$(AR) -rc libgcc.a *.o

LIB1ASMFUNCS = _udivsi3 _divsi3 _umodsi3 _modsi3 _dvmd_tls _call_via_rX

LIB2FUNCS = _muldi3 _divdi3 _moddi3 _udivdi3 _umoddi3 _negdi2 \
_lshrdi3 _ashldi3 _ashrdi3 _ffsdi2 \
_udiv_w_sdiv _udivmoddi4 _cmpdi2 _ucmpdi2 _floatdidf _floatdisf \
_fixunsdfsi _fixunssfsi _fixunsdfdi _fixdfdi _fixunssfdi _fixsfdi

libgcc1.a: lib1thumb.asm
	-rm -f tmplibgcc1.a
# Actually build it in tmplibgcc1.a, then rename at end,
# so that libgcc1.a itself remains nonexistent if compilation is aborted.
	set -e
	for name in $(LIB1ASMFUNCS); \
	do \
	  echo $${name}; \
	  $(CPP) -undef -nostdinc -DL$${name} -x assembler-with-cpp -o $${name}.s lib1thumb.asm; \
	  bash -c 'echo -e ".text\n\t.align\t2, 0\n"' >> $${name}.s ; \
	  $(AS) -mcpu=arm7tdmi -o $${name}.o $${name}.s; \
	  rm -f $${name}.s; \
	  $(AR) -rc tmplibgcc1.a $${name}.o; \
	  rm -f $${name}.o; \
	done
	mv tmplibgcc1.a libgcc1.a

libgcc2.a: libgcc2.c longlong.h
	-rm -f tmplibgcc2.a
# Actually build it in tmplibgcc2.a, then rename at end,
# so that libgcc2.a itself remains nonexistent if compilation is aborted.
	set -e
	for name in $(LIB2FUNCS); \
	do \
	  echo $${name}; \
	  $(CPP) -undef -I ../ginclude -nostdinc -DL$${name} -o $${name}.i libgcc2.c; \
	  $(CC1) -O2 $${name}.i; \
	  rm -f $${name}.i; \
	  bash -c 'echo -e ".text\n\t.align\t2, 0\n"' >> $${name}.s ; \
	  $(AS) -mcpu=arm7tdmi -o $${name}.o $${name}.s; \
	  rm -f $${name}.s; \
	  $(AR) -rc tmplibgcc2.a $${name}.o; \
	  rm -f $${name}.o; \
	done
	mv tmplibgcc2.a libgcc2.a

fp-bit.o: fp-bit.c
	$(CPP) -undef -I ../ginclude -nostdinc -o fp-bit.i fp-bit.c
	$(CC1) -O2 fp-bit.i
	rm -f fp-bit.i
	bash -c 'echo -e ".text\n\t.align\t2, 0\n"' >> fp-bit.s
	$(AS) -mcpu=arm7tdmi -o fp-bit.o fp-bit.s
	rm -f fp-bit.s

dp-bit.o: dp-bit.c
	$(CPP) -undef -I ../ginclude -nostdinc -o dp-bit.i dp-bit.c
	$(CC1) -O2 dp-bit.i
	rm -f dp-bit.i
	bash -c 'echo -e ".text\n\t.align\t2, 0\n"' >> dp-bit.s
	$(AS) -mcpu=arm7tdmi -o dp-bit.o dp-bit.s
	rm -f dp-bit.s

fp-bit.c: fp-bit-base.c
	echo '#define FLOAT' > fp-bit.c
	echo '#define FLOAT_BIT_ORDER_MISMATCH' >> fp-bit.c
	cat fp-bit-base.c >> fp-bit.c

dp-bit.c: fp-bit-base.c
	echo '#define FLOAT_BIT_ORDER_MISMATCH' >> dp-bit.c
	echo '#define FLOAT_WORD_ORDER_MISMATCH' >> dp-bit.c
	cat fp-bit-base.c >> dp-bit.c

.PHONY: clean

clean:
	rm -f *.o *.a *.s *.i
