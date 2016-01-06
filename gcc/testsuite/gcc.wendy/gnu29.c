/*
Date: 25 Jan 89 14:43:05 GMT
From: meyering@cs.utexas.edu  (Jim Meyering)
Subject: 1.32 sun4os4 bug-- passing structures
Message-Id: <150@ai.cs.utexas.edu>
To: bug-gcc@prep.ai.mit.edu
 */

/*
 *	gcc-1.32 (sun4-os4) has a problem passing structure arguments.
 *
 *	If the type FLT is changed to float, all problems disappear.
 *
 *	If PRINTF is defined and -O is *not* used, the program terminates
 *	with a segmentation fault.  In all other cases I've tried, the
 *	program runs, but the arguments to foo are not properly copied.  
 *	To be precise, in foo, the value that one would normally expect
 *	to find in b.y is overwritten with that of b.x. (see output in
 *	comments below)
 */

typedef double FLT;
typedef struct point { FLT x, y; } PT;

#define N 4
FLT x[N];

#if defined(PRINTF)
#define PRINT(a) for (i=0;i<N;i++) printf("%g ", (a)[i]); printf("\n");
#endif

#if defined(FPRINTF) || !defined(PRINTF)
#include <stdio.h>
#define PRINT(a) for (i=0;i<N;i++) fprintf(stdout,"%g ", (a)[i]); \
	fprintf(stdout,"\n");
#endif

	void
main()
{
	int bad,i;
	PT s,t;
	FLT w[N];
	void foo(PT,PT);

	s.x = 0;
	s.y = 1;
	t.x = 999;
	t.y = 3;
	w[0] = s.x; w[1] = s.y;
	w[2] = t.x; w[3] = t.y;

	foo(s,t);

	bad = -1;
	for (i=0;i<N;i++) {
		if (w[i] != x[i]) {
			bad = i;
			break;
		}
	}

	if (bad >= 0) {
		PRINT(w);
		PRINT(x);
	} else {
		printf("Test passed.\n");
	}
	exit(0);
}

	void
foo(PT a,PT b)
{
	extern FLT x[N];
	x[0] = a.x;
	x[1] = a.y;
	x[2] = b.x;
	x[3] = b.y;
}
