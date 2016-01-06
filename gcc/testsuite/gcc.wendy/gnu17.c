#if 0
/*
Date: Mon, 17 Aug 87 09:03:50 PDT
From: bruce@stride.stride.com (Bruce Robertson)
To: bug-gcc@prep.ai.mit.edu
Subject: Optimizer bug in gcc 1.x

I found the cause of the optimizer bug that I've been sending you.  It
occurs because you scan for the first and last occurances of registers
before common subexpression processing, but cse can change these
occurances.  The bug appears in the example program because loop
optimization removes a load of "mem" into a pseudo-register, because
based on the information of the register scan it thinks that there are
no more appearances of the pseudo-register.  Cse added a reference,
however, and this information is not placed in regno_last_uid[].  The
solution seems to be to call reg_scan() again after calling
cse_main().  It fixed the bug anyway, and didn't seem to cause
anything bad.

Diffs for the fix, and the original sample program follow.


Diffs:
======================================================================
*** /tmp/,RCSt1a00495	Thu Aug 13 13:09:01 1987
--- toplev.c	Thu Aug 13 13:08:39 1987
***************
*** 776,781 ****
--- 776,786 ----
  
        TIMEVAR (cse_time, tem = cse_main (insns, max_reg_num ()));
  
+       /* We need to scan for registers again, because common subexpression
+ 	 elimination could have rearranged the first and last occurrances
+ 	 of registers. */
+       TIMEVAR (cse_time, reg_scan (insns, max_reg_num ()));
+ 
        if (tem)
  	TIMEVAR (jump_time, jump_optimize (insns, 0, 0));
      }



Sample program:
======================================================================
#endif

typedef unsigned char quarterword;
typedef unsigned short halfword;

typedef union
{ 
    struct { 
        halfword    lh; 
        halfword    rh; 
    } hh1; 
    struct { 
        quarterword b0; 
        quarterword b1; 
        halfword    rh; 
    } hh2; 
}
    two_halves;

typedef struct
{ 
    quarterword b0;
    quarterword b1;
    quarterword b2;
    quarterword b3;
}
    four_quarters;

typedef union
{ 
    int         i;   
    float      gr;  
    two_halves      hh;  
    four_quarters   qqqq;
}
    memory_word;

memory_word     mem[10];

unsigned int fewest_demerits;
unsigned short best_bet = 0;
unsigned short best_line = -1;


void
foo(r)
	unsigned short r;
{

	do {
		if (mem[r].hh.hh2.b0 != 2 &&
		    mem[(r) + 2].i < fewest_demerits) {
                        best_bet = r;
		}
		r = mem[r].hh.hh1.rh;
	} while (r != (65501 - 7));

	/*
	 * This is the statement that is causing trouble.  See the comment
	 * in the assembly code below.
	 */
	best_line = mem[best_bet + 1].hh.hh1.lh;
}

main()
{
	mem[0].hh.hh2.b0 = 0;
	mem[0].hh.hh1.rh = 65501-7;
	mem[1].hh.hh1.lh = 456;
	mem[2].i = 30;
	foo(0);
	if (best_line == 456)
		printf("Test passed\n");
	else
		printf("Test failed, best_line = %d\n", best_line);
}

/*

#NO_APP
.comm _fewest_demerits,4
.comm _best_bet,2
.comm _best_line,2
.globl _foo
.text
	.even
_foo:
	link a6,#0
	moveml #0x3000,sp@-
	movew #1,d0
	clrl d1
	clrl d2
	lea _mem,a0
L2:
	movew d0,d1
	cmpb #2,a0@(d1:l:4)
	jeq L4
	movel a0@(8,d1:l:4),d3
	cmpl _fewest_demerits,d3
	jcc L4
	movew d0,_best_bet
L4:
	movew d0,d2
	movew a0@(2,d2:l:4),d0
	cmpw #65494,d0
	jne L2
	clrl d0
	movew _best_bet,d0

|
|	Where did this reference (below) to A1 come from?  It should be A0,
|	since that still points to "mem".  A1 is never set in the code
|	above.
|
|	When this code fragment is in place in Common Tex (in function
|	"line_break()" in file "par.c") the reference is to -4(fp)
|	rather than A1.
|

	movew a1@(4,d0:l:4),_best_line
	moveml a6@(-8),#0xc
	unlk a6
	rts
*/
