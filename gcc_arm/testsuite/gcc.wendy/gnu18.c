/*
Date: Sat, 5 Sep 87 00:34:13 PDT
From: gnu (John Gilmore)
To: rms
Subject: Stack adjustment bug in 'continue' -- gcc 1.10+

Compiling the following simplified program with or without -O produces
bad code.  The 'continue' compiles to a jump to a label which PRECEDES
the stack adjust for the printf at the end of the loop.  This makes the
stack pointer walk up each time around the loop.  If you compile without
-O, "foo" gets allocated on the stack and the debug printf fires.
Compiled with -O, it coredumps before printing.  Compiled on a working
compiler, it loops forever.

I have installed the bugfix for while loops posted to bug-gcc; it fails
with or without the bugfix.
*/

int	tty;

iscmd() {
	static int counter;

	if (counter++ < 10000) return 0;
	printf("Test passed\n");
	exit(0);
}

void
main()
{
	int foo = 0;

	for ( ;; ) {
		if (foo != 0) printf("***BUG*** foo=%x\n", foo);
		if (!tty) {
			if (!iscmd ())
				continue;
			foo = 1;
		}

		printf("**BUG** Past iscmd\n");
	}

	return;
}
