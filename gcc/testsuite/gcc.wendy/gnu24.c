/* From Sun-Spots v6n12
Date:    26 Jan 88 15:58:58 CST (Tue)
From:    uunet!mcrware!jejones@ut-sally.UUCP
Subject: ?: vs. pointer to function "returning" void

I find that on a Sun 3, the C compiler complains about code of the following
form with a message "operands of : have incompatible types."  I don't think
this is correct, because woof1 and woof2 are both pointers to functions
"returning" void, so that (pointer to function returning void) should be
the type of the actual parameter.  Right?

Have other people seen this problem?  Is it, if it is indeed a problem
with the Sun C compiler, fixed in some release?

                James Jones
*/

void    woof1() {};
void	woof2() {};

main(i, argv)
int     i;
char **argv;
{
	bletch(i ? woof1 : woof2);
	printf("Test passed (if it compiled at all)\n");
}

bletch(x)
	void (*x)();
{
}
