ifeq ($(prefix),)
CHECK_PREFIX := @printf "\033[31;1m*******************************\nUse 'make install prefix=\"path\"\n*******************************\033[0m\n"; exit 1
else
CHECK_PREFIX := @:
PREFIX := $(shell realpath $(prefix))
endif

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

CONFIGURE_ARGS := SHELL="$(SHELL)" LDFLAGS="" CFLAGS="-O3 -g -march=native -mtune=native" CC="$(CC)" --disable-plugins --without-libtool --without-libintl --target=armv4tl-none-eabi --program-prefix=arm-none-eabi- --disable-dependency-tracking --enable-gold=no --with-system-zlib --without-isl --exec-prefix=NONE CC="$(CC)"
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

all: binutils old_gcc gcc libc libgcc

binutils: $(BINUTILS_TGTS)

install: install-prefix-check binutils old_gcc gcc libc libgcc $(BINUTILS_INSTALL_STRIP)
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

clean: binutils_clean $(CLEAN) libc_clean libgcc_clean old_gcc_clean gcc_clean


.PHONY: binutils old gcc old_gcc libc libgcc all clean install
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
bfd-headers: bfd-configure
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

