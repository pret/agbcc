/*
Message-Id: <8712292244.AA24155@prep.ai.mit.edu>
Date: Mon, 28 Dec 87 21:05:20 PST
From: Greg Satz <satz@clutter.cisco.com>
To: bug-gcc@prep.ai.mit.edu
Subject: Error handling bug?

The following program causes cc1 to make a core file.  Since it was
caused by a syntax error, I didn't spend that much time tracking it
down.  I was able to reduce the example to this small result.  Could
someone check to see if this bug exists in the Vax and Sun versions as
well and let me know?  Thanks!
*/

int global;
main()
{
    int ch;

  if (global) {
    printf("here");
  } else {
    if (((ch & 0x7f) == 0)) && (ch & 1)) { /* syntax error here */
	if (!ch)
	    return(0);
	ch = 0;
    }
  }
}

