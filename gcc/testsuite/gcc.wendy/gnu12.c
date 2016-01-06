/*
Deceived: by hoptoad.uucp (1.1/SMI-3.0DEV3)
	id AA08037; Wed, 1 Apr 87 03:40:07 PST
Message-Id: <8704010914.AA23555@prep.ai.mit.edu>
Date: 31 Mar 1987 2359-PST (Tuesday)
From: Malcolm Wing <wing@sonoma.stanford.edu>
To: bug-gcc@prep.ai.mit.edu
Subject: Clobber nil bug and Tail recursion bug

BUG 2
    When compiling Ackerman for the vax it tries to remove the
tail recursion. However the tail recursive call contains another
call so the update (used to be parameter passing) can't be done
until the imbedded call returns.

SOURCE

/* Ackerman's function */
main()
{
	int i;

	i = A(3,6);
	if (i == 509)
		printf("Test passed\n");
	else
		printf("FAILED ackerman's(3, 6): %d\n", i);
}

A(x,y)
int x,y;
{
    if(x==0) return(++y);
    if(y==0) return(A(--x,1));
    return(A(x-1,A(x,--y)));
    }
