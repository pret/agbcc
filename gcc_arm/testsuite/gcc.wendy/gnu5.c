/*
Date: 22 Dec 87 08:38:59 PST (Tuesday)
Subject: Serious bug in gcc 1.16
From: "James_L_Mayer.WBST128"@xerox.com
To: bug-gcc@prep.ai.mit.edu
Message-Id: <871222-084137-7045@Xerox>

Here is a bug in release 1.16 of gcc running on a Sun3:

Problem:
	In assembler output, "pointer" is incremented twice for each iteration.
	(when compiled optimized.)
*/

struct bits
{
	unsigned bit0: 1;
	unsigned bit1: 1;
} foo[2];

alpha(pointer, count)
	struct bits *pointer;
	int count;
{
	while (--count >= 0)
	{
		pointer->bit0 = !pointer->bit0;
		pointer++;
	}
	
	if (pointer == &foo[2])
		printf("Test passed\n");
	else
		printf("FAILED, pointer=%x, &foo[2]=%x\n", pointer, &foo[2]);
}

main()
{
	alpha(foo, 2);
	return 0;
}
