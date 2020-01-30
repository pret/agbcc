int install = !0;	/* Gcc 1.16 said "initializer is not constant" */
			/* Gcc 1.17 gets wrong answer! */
char foo = !1;
int bar = !3.14156;	/* SunOS 3.3 cc said "Illegal initialization" */
int pno = ! &bar;	/* Gcc 1.17 said "initializer for static variable
			   uses complex arithmetic" */
int pyes = !(char *)0;

main() {
	if (install && !foo && !bar && pyes && !pno)
		printf("Test passed\n");
	else
		printf("FAILED, install=%d, foo=%d, bar=%d, pno=%d, pyes=%d\n",
			install, foo, bar, pno, pyes);
}

