/*
From ptsfa!ames!sdcsvax!nosc!cod!owen Mon Dec 28 14:54:53 1987
From: owen@cod.NOSC.MIL (Wallace E. Owen)
Newsgroups: comp.lang.c
Subject: msc bitfield bug
Message-ID: <934@cod.NOSC.MIL>
Date: 28 Dec 87 22:54:53 GMT
Organization: Naval Ocean Systems Center, San Diego

The following small bug in Microsoft 'C' (4.0) handling of forcing
a bitfield to the next int or long was revealed while developing a Large
piece of software. It's in the form of a small program which reveals the
bug:

/*
   This short program displays a bug in MS C buildout of bitfield expressions.
   Problem: Forcing bit field alignment to the next unsigned int or unsigned
   long works if the next statement is a bitfield expression, but if it's
   a unsigned long for instance, the padding ':0;' forces an extra item to
   be allocated BETWEEN the two expressions. Must be the method used
   to force bitfield to the next unsigned int or unsigned long. Should check
   that indeed the next statement IS a bitfield expression.
*/

struct testa			/* 8 bytes */
{
	unsigned long
		a: 5, b: 5, c: 5, d: 5, e: 5, : 0;
	unsigned long
		f: 8, g: 8, h: 8, i: 8;
};

struct testb				/* Should be 8 bytes, but it's 12! */
{
	unsigned long
		a: 5, b: 5, c: 5, d: 5, e: 5, : 0;
	unsigned long f;
};

main()
{
	/* Print sizes of things */
	if (sizeof (struct testa) == sizeof (struct testb)) {
		printf("Test passed\n");
	} else {
		printf("Size of struct testa: %u\n", sizeof( struct testa));
		printf("Size of struct testb: %u\n", sizeof( struct testb));
	}
}

/*
I'm going to try this with 5.0- Also, while at the c++ conference in
Santa Fe, met the author of MSC 5.0's front end, and showed him this code.
		-- Wallace Owen
		(owen@cod.nosc.mil)
		(619) 553-3567
*/
