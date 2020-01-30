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
	int i, j;

	i = A(3,6);
	j = B(3,6);
	if (i == 509 && j == 509 && i == j)
		printf("Test passed\n");
	else
		printf("FAILED ackerman's(3, 6): %d, %d\n", i, j);
}

A(x,y)
int x,y;
{
    if(x==0) return(++y);
    if(y==0) return(A(--x,1));
    return(A(x-1,A(x,--y)));
}

/* This one is more in the true spirit of the original Algol code. */
B(m,n)
int m,n;
{
  return m==0 ? n+1 : n==0 ? B(m-1,1) : B(m-1,B(m,n-1));
}

/* Try it with subtract/add rather than ++ and -- */
C(x,y)
int x,y;
{
    if(x==0) return(y+1);
    if(y==0) return(C(x-1,1));
    return(C(x-1,C(x,y-1)));
}
