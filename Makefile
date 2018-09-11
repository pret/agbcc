ifeq ($(prefix),)
CHECK_PREFIX := @printf "\033[31;1m*******************************\nUse 'make install prefix=\"path\"\n*******************************\033[0m\n"; exit 1
else
CHECK_PREFIX := @:
PREFIX := $(shell realpath $(prefix))
endif


MAKEFLAGS += --no-print-directory

ifeq (Windows_NT,$(OS))
EXE := .exe
else
EXE :=
endif

all: binutils old_gcc gcc libc libgcc

install: binutils old_gcc gcc libc libgcc
	$(CHECK_PREFIX)
	@mkdir -p $(PREFIX)/tools/agbcc
	@mkdir -p $(PREFIX)/tools/agbcc/bin
	@mkdir -p $(PREFIX)/tools/agbcc/include
	@mkdir -p $(PREFIX)/tools/agbcc/lib
	@mkdir -p $(PREFIX)/tools/binutils
	@mkdir -p $(PREFIX)/tools/binutils/bin
	@mkdir -p $(PREFIX)/tools/binutils/lib
	cp agbcc $(PREFIX)/tools/agbcc/bin/
	cp old_agbcc $(PREFIX)/tools/agbcc/bin/
	cp -R libc/include $(PREFIX)/tools/agbcc/ #drop include, because we don't want include/include
	cp ginclude/* $(PREFIX)/tools/agbcc/include/
	cp libgcc.a $(PREFIX)/tools/agbcc/lib/
	cp libc.a $(PREFIX)/tools/agbcc/lib/
	@$(MAKE) -C binutils install-strip prefix="$(PREFIX)/tools/binutils"



old_agbcc$(EXE): agbcc$(EXE)
	@$(MAKE) -C gcc tidy
	@$(MAKE) -C gcc old
	cp gcc/old_agbcc$(EXE) old_agbcc$(EXE)

old: old_agbcc$(EXE)
old_gcc: old

gcc_clean:
	$(RM) agbcc$(EXE)
	@$(MAKE) -C gcc clean

old_gcc_clean:
	$(RM) old_agbcc$(EXE)
	@$(MAKE) -C gcc clean

agbcc$(EXE):
	@$(MAKE) -C gcc tidy
	@$(MAKE) -C gcc
	cp gcc/agbcc$(EXE) agbcc$(EXE)

# Careful with these ./configure flags, they are very touchy.
binutils/Makefile:
	cd binutils && touch **/* && $(SHELL) ./configure CONFIG_SHELL="$(SHELL)" SHELL="$(SHELL)" LDFLAGS="-g" CFLAGS="-O2 -g3" CC="$(CC)" --disable-plugins --without-libtool --without-libintl --target=armv4tl-none-eabi --program-prefix=arm-none-eabi- --disable-dependency-tracking --enable-gold=no --with-system-zlib --without-isl

binutils: binutils/Makefile
	@$(MAKE) -C binutils

binutils_clean:
	@[ ! -f binutils/Makefile ] || $(MAKE) -C binutils clean
	find binutils -name "Makefile" -o -name "config.cache" -exec rm -rf {} \;

libc.a: old_gcc binutils
	@$(MAKE) -C libc
	cp libc/libc.a libc.a

libc: libc.a

libc_clean:
	$(RM) libc.a
	@$(MAKE) -C libc clean

libgcc.a: old_gcc binutils
	@$(MAKE) -C libgcc
	cp libgcc/libgcc.a libgcc.a

libgcc: libgcc.a

libgcc_clean:
	$(RM) libgcc.a
	@$(MAKE) -C libgcc clean

clean: binutils_clean libc_clean libgcc_clean old_gcc_clean gcc_clean


.PHONY: binutils old gcc old_gcc libc libgcc all clean install
