/* From Sun-Spots v6n11
Date:    Fri, 22 Jan 88 10:07:18 EST
From:    im4u!rutgers!rochester!srs!matt@ut-sally.UUCP
Subject: Bug in C compiler

We have had a bug in our main signal analysis tool for well over a year.
Occasionally, and apparently w/o any regularity, one of the graphs (a
raster display of FFTs of an input signal) would produce what looked like
garbage, but tended to follow the basic trend of the input data.
Recompiling the tool sometimes changed the frequency of occurance and with
the latest changes, it tended to occur quite often.  Well, I have finally
tracked it down to a compiler problem and this bug will bother us no
more...

Release:  Sun OS 3.2
Systems:  Sun3, Sun2
Comment:  The following program outputs (erroneously) 0xff00 for the
	  second call to screwit().  Basically, the compiler fails to
	  clear the upper half of "d7" before adding it into "a".
	  "a" can be a signed or unsigned short.  If "a" is an int
	  (long), it appears to work correctly.

gnu@toad.com comment:  Appears to only fail this with without -O, probably
	because the stores to d7 in main() are removed by c2.
*/
#include <stdio.h>
static int fail;

main()
{
    register short d7;
    unsigned char b = 0;

    d7 = 0x00ff;
    screwit(&b);
    d7 = 0xffff;
    screwit(&b);
    if (!fail)
	printf("Test passed\n", d7);
    else
	printf("Test FAILED\n", d7);
}

screwit(b)
register unsigned char *b;
{
    register unsigned char d7;
    register unsigned short a = 0;

    d7 = *b; 
    a += (unsigned short) d7;
    if (a == 0) return;
    printf("C: 0x%04x\n", a);
    fail++;
}

/*
UUCP:	{allegra,rutgers,ames}!rochester!srs!matt	Matt Goheen
	"First the pants, THEN the shoes."		S.R. Systems
*/
