.PHONY = agbcc old_agbcc agbcc_arm libc.a libgcc.a

# needed for out-of-tree builds of agbcc and old_agbcc
export srcdir = ../gcc

all: agbcc old_agbcc agbcc_arm libc.a libgcc.a

clean:
	rm -rf build_agbcc build_old_agbcc
	-$(MAKE) -C gcc_arm distclean
	$(MAKE) -C libc clean
	$(MAKE) -C libgcc clean

agbcc:
	mkdir -p build_agbcc
	$(MAKE) -C build_agbcc -f ../gcc/Makefile clean
	$(MAKE) -C build_agbcc -f ../gcc/Makefile normal

old_agbcc:
	mkdir -p build_old_agbcc
	$(MAKE) -C build_old_agbcc -f ../gcc/Makefile clean
	$(MAKE) -C build_old_agbcc -f ../gcc/Makefile old

agbcc_arm:
	-$(MAKE) -C gcc_arm distclean
	cd gcc_arm && ./configure --target=arm-elf --host=i386-linux-gnu
	$(MAKE) -C gcc_arm cc1

libc.a: old_agbcc
	$(MAKE) -C libc clean
	$(MAKE) -C libc

libgcc.a: old_agbcc
	$(MAKE) -C libgcc clean
	$(MAKE) -C libgcc

install: all
ifeq ($(PREFIX),)
	$(error PREFIX is empty! Specify a PREFIX to install to)
endif
	$(info Installing into $(PREFIX))
	mkdir -p $(PREFIX)/tools/agbcc
	mkdir -p $(PREFIX)/tools/agbcc/bin
	mkdir -p $(PREFIX)/tools/agbcc/include
	mkdir -p $(PREFIX)/tools/agbcc/lib
	cp build_agbcc/agbcc $(PREFIX)/tools/agbcc/bin/
	cp build_old_agbcc/old_agbcc $(PREFIX)/tools/agbcc/bin/
	cp gcc_arm/cc1 $(PREFIX)/tools/agbcc/bin/agbcc_arm
	@# drop include, because we don't want include/include
	cp -R libc/include $(PREFIX)/tools/agbcc/
	cp ginclude/* $(PREFIX)/tools/agbcc/include/
	cp libgcc/libgcc.a $(PREFIX)/tools/agbcc/lib/
	cp libc/libc.a $(PREFIX)/tools/agbcc/lib/
	
