/* DEJAGNU_OUTPUT:"Test failed (if it compiles at all)" */

/*
 * This program should generate at least a warning; preferably an error.
 */
int *
main()
{ 
   int local = 1; 

   printf("Test failed (if it compiles at all)\n");

   return &local;
}

/*
 * RMS comments:
 *
Date: Sun, 18 Dec 88 14:39:18 EST
From: rms@wheaties.ai.mit.edu (Richard Stallman)
To: gnu@toad.com
Subject: gcc-1.31 sparc test suite results

I do not anticipate that gnu27.c will ever produce a warning
message.  I can see kludgy ways to produce them in simple cases,
but more complicated circumlocutions would avoid them.  In general
the problem is uncomputable.
 */
