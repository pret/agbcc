/*
Message-Id: <9010022328.AA01576@shark2>
To: cygint!bugs
Subject: bug in gcc front end
Date: Tue, 02 Oct 90 16:28:14 PDT
From: @hermes.intel.com:kevins@shark2

The following source program causes gcc to emit these errors:
t1.c:3: bit-field `a' width not an integer constant
t1.c:4: bit-field `b' width not an integer constant
**** test ****/
static struct b {	/* 3.4 (cont.) */
	int i;
	unsigned int a : (1 + 3 * 2) / 7;
	unsigned int b : (1 + 3 * 2) / 7;
	} b;
/**** end test ****
This program is legal in that ANSI (well Dec 7, 1988) 3.5.2.1 grammar
allows constant expressions here.
*/

#include <stdio.h>
main()
{
	printf("Test passed.\n");
}
