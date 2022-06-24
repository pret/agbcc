make -C gcc clean
# || true is needed to keep going if the distclean fails, such as when no configure has been done before
make -C gcc_arm distclean || true
make -C libgcc clean
make -C libc clean
rm -f agbcc old_agbcc agbcc_arm libc.a libgcc.a
