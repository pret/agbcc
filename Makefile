ifeq ($(prefix),)
CHECK_PREFIX := @printf "\033[31;1m*******************************\nUse 'make install prefix=\"path\"\n*******************************\033[0m\n"; exit 1
else
CHECK_PREFIX := @:
PREFIX := $(shell realpath $(prefix))
endif

CFLAGS ?= -O3 -g -march=native -mtune=native

# Simulate autotools by hacking recursive submake
ifeq (,$(shell which aclocal-1.16 2>/dev/null || true))
ENABLE_AUTOTOOLS =
DO_AUTORECONF = $(error Please install autotools first!)
else
ENABLE_AUTOTOOLS ?= 1
DO_AUTORECONF := autoreconf -f && { command -v untermux >/dev/null && untermux configure; }
endif

ifeq (,$(ENABLE_AUTOTOOLS))
export AUTOMAKE := touch Makefile.in;:
export AUTOCONF := touch configure;:
export AUTOHEADER := touch config.h.in;:
export ACLOCAL := touch aclocal.m4;:
SUBSUBMAKEFLAGS := 	AUTOMAKE='$(AUTOMAKE)' \
	AUTOCONF='$(AUTOCONF)' \
	AUTOHEADER='$(AUTOHEADER)' \
	ACLOCAL='$(ACLOCAL)'
SUBMAKE := MAKE="$(MAKE) --no-print-directory $(SUBSUBMAKEFLAGS) "
else
SUBMAKE :=
endif

SUBDIRS := ld binutils gas bfd libiberty
BINUTILS_TGTS := $(addsuffix -all, $(SUBDIRS))
CONFIGURE_TGTS := $(addsuffix -configure, $(SUBDIRS))
OBJS_TGTS := $(addsuffix -objs, $(SUBDIRS))
CLEAN := $(addsuffix -clean, $(SUBDIRS))

CONFIGURE_ARGS := SHELL="$(SHELL)" LDFLAGS="" CFLAGS="$(CFLAGS)" CC="$(CC)" --disable-plugins --without-libtool --without-libintl --target=armv4tl-none-eabi --program-prefix=arm-none-eabi- --disable-dependency-tracking --enable-gold=no --with-system-zlib --without-isl --exec-prefix=NONE CC="$(CC)"
CONFIGURE := $(SHELL) ./configure -C --disable-option-checking $(CONFIGURE_ARGS)

INSTALL_SUBDIRS := ld binutils gas
BINUTILS_INSTALL := $(addsuffix -install, $(INSTALL_SUBDIRS))
BINUTILS_INSTALL_STRIP := $(addsuffix -install-strip, $(INSTALL_SUBDIRS))

prefix ?=

# The dash shell (/bin/sh on ubuntu) does not support $LINENO and messes up configure scripts.
DASH_LINENO_CHECK := $(shell $(SHELL) -c 'echo $$LINENO')
ifeq (,$(DASH_LINENO_CHECK))
  ifneq (,$(shell which mksh))
    override SHELL := $(shell which mksh)
  else
    override SHELL := $(shell which bash)
  endif
endif
export SHELL := $(SHELL)
MAKEFLAGS += --no-print-directory

ifeq (Windows_NT,$(OS))
EXE := .exe
else
EXE :=
endif

all: binutils old_gcc gcc libc libgcc libagb_flash libagbsyscall libgcnmultiboot libisagbprn librfu libm4a libsiirtc

binutils: $(BINUTILS_TGTS)

install: install-prefix-check binutils old_gcc gcc libc libgcc libagb_flash libagbsyscall libgcnmultiboot libisagbprn librfu libm4a libsiirtc $(BINUTILS_INSTALL_STRIP)
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
	cp -R ginclude/* $(PREFIX)/tools/agbcc/include/
	cp agb_flash/agb_flash.h $(PREFIX)/tools/agbcc/include/
	cp librfu/librfu.h $(PREFIX)/tools/agbcc/include/
	cp libgcnmultiboot/libgcnmultiboot.h $(PREFIX)/tools/agbcc/include/
	cp siirtc/siirtc.h $(PREFIX)/tools/agbcc/include
	cp libgcc.a $(PREFIX)/tools/agbcc/lib/
	cp libc.a $(PREFIX)/tools/agbcc/lib/
	cp libagb_flash.a $(PREFIX)/tools/agbcc/lib/
	cp libagbsyscall.a $(PREFIX)/tools/agbcc/lib/
	cp libgcnmultiboot.a $(PREFIX)/tools/agbcc/lib/
	cp libisagbprn.a $(PREFIX)/tools/agbcc/lib/
	cp librfu_1024.a $(PREFIX)/tools/agbcc/lib/
	cp librfu_1026.a $(PREFIX)/tools/agbcc/lib/
	cp libm4a.a $(PREFIX)/tools/agbcc/lib/
	cp libsiirtc.a $(PREFIX)/tools/agbcc/lib/


autoreconf:
	for i in **/configure.ac; do \
		cd $$(dirname $$i); \
		$(DO_AUTORECONF); \
		cd -; \
	done

old_agbcc$(EXE): agbcc$(EXE)
	@$(MAKE) -C gcc tidy
	@$(MAKE) -C gcc old BASE_CFLAGS="-DAGBCC_VERSION=1 $(CFLAGS)"
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
	@$(MAKE) -C gcc BASE_CFLAGS="-DAGBCC_VERSION=1 $(CFLAGS)"
	cp gcc/agbcc$(EXE) agbcc$(EXE)


binutils_clean: $(CLEAN)

libc.a: libc/libc.a
	cp libc/libc.a libc.a
libc/libc.a: old_gcc binutils
	@$(MAKE) -C libc

libc: libc.a

libc_clean:
	$(RM) libc.a
	@$(MAKE) -C libc clean

libgcc.a: libgcc/libgcc.a
	cp libgcc/libgcc.a libgcc.a

libgcc/libgcc.a: old_gcc binutils
	@$(MAKE) -C libgcc

libgcc: libgcc.a

libgcc_clean:
	$(RM) libgcc.a
	@$(MAKE) -C libgcc clean

libagb_flash.a: agb_flash/libagb_flash.a
	cp $< $@
agb_flash/libagb_flash.a: agbcc
	@$(MAKE) -C agb_flash

libagb_flash: libagb_flash.a

libagb_flash_clean:
	$(RM) libagb_flash.a
	@$(MAKE) -C agb_flash clean

libagbsyscall.a: libagbsyscall/libagbsyscall.a
	cp $< $@
libagbsyscall/libagbsyscall.a:
	@$(MAKE) -C libagbsyscall

libagbsyscall: libagbsyscall.a

libagbsyscall_clean:
	$(RM) libagbsyscall.a
	@$(MAKE) -C libagbsyscall clean

libgcnmultiboot.a: libgcnmultiboot/libgcnmultiboot.a
	cp $< $@
libgcnmultiboot/libgcnmultiboot.a:
	@$(MAKE) -C libgcnmultiboot

libgcnmultiboot: libgcnmultiboot.a

libgcnmultiboot_clean:
	$(RM) libgcnmultiboot.a
	@$(MAKE) -C libgcnmultiboot clean

libisagbprn.a: libisagbprn/libisagbprn.a
	cp $< $@
libisagbprn/libisagbprn.a: agbcc
	@$(MAKE) -C libisagbprn

libisagbprn: libisagbprn.a

libisagbprn_clean:
	$(RM) libisagbprn.a
	@$(MAKE) -C libisagbprn clean

librfu_%.a: librfu/librfu_%.a
	cp $< $@
librfu/librfu_%.a: TARGET = $(@:librfu/librfu_%.a=v%)
librfu/librfu_%.a: agbcc
	@$(MAKE) -C librfu clean
	@$(MAKE) -C librfu $(TARGET)

librfu: librfu_1024.a librfu_1026.a

librfu_clean:
	$(RM) librfu_1024.a librfu_1026.a
	@$(MAKE) -C librfu clean

libm4a.a: m4a/libm4a.a
	cp $< $@
m4a/libm4a.a: old_agbcc
	@$(MAKE) -C m4a

libm4a: libm4a.a

libm4a_clean:
	$(RM) libm4a.a
	@$(MAKE) -C m4a clean

libsiirtc.a: siirtc/libsiirtc.a
	cp $< $@
siirtc/libsiirtc.a: agbcc
	@$(MAKE) -C siirtc

libsiirtc: libsiirtc.a

libsiirtc_clean:
	$(RM) libsiirtc.a
	@$(MAKE) -C siirtc clean

clean: binutils_clean $(CLEAN) libc_clean libgcc_clean old_gcc_clean gcc_clean libagb_flash_clean libagbsyscall_clean libgcnmultiboot_clean libisagbprn_clean librfu_clean libm4a_clean libsiirtc_clean


.PHONY: binutils old gcc old_gcc libc libgcc libagb_flash libagbsyscall libgcnmultiboot libisagbprn librfu librfu libm4a libsiirtc all clean install
.PHONY: install-prefix-check $(ALL) $(SUBDIRS) $(OBJS_TGTS) $(BINUTILS_INSTALL) $(BINUTILS_INSTALL_STRIP) $(CLEAN) $(CONFIGURE_TGTS) install install-strip clean


# Stupidity check for make install. Especially check for Termux,
# because it actually sets $PREFIX in the environment.
install-prefix-check:
	@if [ -z "$(PREFIX)" ]; then				\
	    echo "Invalid prefix!" >&2;				\
	    exit 1;						\
	else							\
	    case $(PREFIX) in					\
	    / | /usr* | /data/data/com.termux/files/usr*)	\
	        echo "$(PREFIX): Invalid prefix!" >&2;		\
	        exit 1;						\
	       ;;						\
	    esac						\
	fi

configure: ld-configure gas-configure bfd-configure libiberty-configure binutils-configure

# dir-configure:
#     runs ./configure if Makefile doesn't exist.
# dir-objs:
#     Builds all of the objects without linking. Even though these
#     subdirs all ultimately depend on libiberty and libbfd, they
#     don't actually need them until linking. We can save time in
#     parallel builds by avoiding this wait.
# dir-all:
#     Builds the final step. This is the part that needs libbfd.
# dir-clean:
#     Cleans. Duh.
# dir-install, dir-install-strip:
#     Installs, optionally stripping.

ld-configure:
	@if [ ! -f ld/Makefile ]; then 				\
	    echo "Configuring in ld...";			\
	    cd ld && $(CONFIGURE);				\
	fi
ld-objs: ld-configure bfd-headers
	@$(MAKE) -C ld objs $(SUBSUBMAKEFLAGS)
ld-all: ld-objs bfd-all libiberty-all
	@$(MAKE) -C ld $(SUBSUBMAKEFLAGS)
ld-clean: ld-configure
	@$(MAKE) -C ld clean $(SUBSUBMAKEFLAGS)
ld-install: ld-all
	@$(MAKE) -C ld install prefix=$(PREFIX)/tools/binutils $(SUBSUBMAKEFLAGS)
ld-install-strip: ld-all
	@$(MAKE) -C ld install-strip prefix=$(PREFIX)/tools/binutils $(SUBSUBMAKEFLAGS)

binutils-configure:
	@if [ ! -f binutils/Makefile ]; then 			\
	    echo "Configuring in binutils...";			\
	    cd binutils && $(CONFIGURE); 			\
	fi
binutils-objs: binutils-configure bfd-headers
	@$(MAKE) -C binutils objs $(SUBSUBMAKEFLAGS)
binutils-all: binutils-objs bfd-all libiberty-all
	@$(MAKE) -C binutils $(SUBSUBMAKEFLAGS)
binutils-clean: binutils-configure
	@$(MAKE) -C binutils clean $(SUBSUBMAKEFLAGS)
binutils-install: binutils-all
	@$(MAKE) -C binutils install prefix=$(PREFIX)/tools/binutils  $(SUBSUBMAKEFLAGS)
binutils-install-strip: binutils-all
	@$(MAKE) -C binutils install-strip prefix=$(PREFIX)/tools/binutils $(SUBSUBMAKEFLAGS)

gas-configure:
	@if [ ! -f gas/Makefile ]; then 			\
	    echo "Configuring in gas...";			\
	    cd gas && $(CONFIGURE);				\
	fi
gas-objs: gas-configure bfd-headers
	@$(MAKE) -C gas objs $(SUBSUBMAKEFLAGS)
gas-all: gas-objs bfd-all libiberty-all
	@$(MAKE) -C gas $(SUBSUBMAKEFLAGS)
gas-clean: gas-configure
	@$(MAKE) -C gas clean $(SUBSUBMAKEFLAGS)
gas-install: gas-all
	@$(MAKE) -C gas install prefix=$(PREFIX)/tools/binutils $(SUBSUBMAKEFLAGS)
gas-install-strip: gas-all
	@$(MAKE) -C gas install-strip prefix=$(PREFIX)/tools/binutils $(SUBSUBMAKEFLAGS)

bfd-configure:
	@if [ ! -f bfd/Makefile ]; then				\
	    echo "Configuring in bfd...";			\
	    cd bfd && $(CONFIGURE);				\
	fi
bfd-objs: bfd-configure
	@$(MAKE) -C bfd objs $(SUBSUBMAKEFLAGS)
bfd-all: bfd-objs
	@$(MAKE) -C bfd $(SUBSUBMAKEFLAGS)
bfd-clean: bfd-configure
	@$(MAKE) -C bfd clean $(SUBSUBMAKEFLAGS)

# All subdirs (aside from bfd and libiberty) need bfd.h to build.
bfd-headers: bfd/stmp-bfd-h
bfd/stmp-bfd-h: bfd-configure
	@$(MAKE) -C bfd headers $(SUBSUBMAKEFLAGS)
.PHONY: bfd-headers

libiberty-configure:
	@if [ ! -f libiberty/Makefile ]; then 			\
	    echo "Configuring in libiberty...";			\
	    cd libiberty && $(CONFIGURE);			\
	fi
libiberty-objs: libiberty-configure
	@$(MAKE) -C libiberty objs $(SUBSUBMAKEFLAGS)
libiberty-all: libiberty-objs
	@$(MAKE) -C libiberty $(SUBSUBMAKEFLAGS)
libiberty-clean: libiberty-configure
	@$(MAKE) -C libiberty clean $(SUBSUBMAKEFLAGS)

