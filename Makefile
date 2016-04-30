.PHONY: all clean

ifeq ($(OS),Windows_NT)
    AGBCC := agbcc.exe
else
    AGBCC := agbcc
endif

all: gcc/$(AGBCC) libgcc/libgcc.a

gcc/$(AGBCC):
	make -C gcc

libgcc/libgcc.a: | gcc/$(AGBCC)
	make -C libgcc

clean:
	make -C gcc clean
	make -C libgcc clean
