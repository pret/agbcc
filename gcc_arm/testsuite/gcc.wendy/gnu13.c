/*
Date: Tue, 21 Apr 87 09:58:10 PDT
From: aks@acc-sb-unix.arpa (Alan Stebbens)
To: bug-gcc@PREP.AI.MIT.EDU
Subject: Optimizer doesn't.

Dear GCC guys:

The following simple C program illustrates a problem with the
current version of GCC.  What happens is that D3 is used doubly:
once for loop limit testing, and once for long constant stores.
Also, a loop limit test is being performed for the first iteration
even when both the initial value and the ending values are known
constants.  Most optimizers can avoid the initial test if these
values are known.

Thanks for your time.

Alan Stebbens (aks@acc-sb-unix.ARPA)

==================== tst1.c ====================
*/
int array[100];
int darray[10][10];

main()
{
   register int i, j;
   for (i = 0; i < 100; i++) array[i] = 3;
   for (i = 0; i < 10; i++)
      for (j = 0; j < 10; j++)
	 darray[i][j] = 3;
   return 0;
}

/*
==================== tst1.s ====================
.comm _array,400
.comm _darray,400
.globl _main
.text
	.even
_main:
	link a6,#0
	moveml #0x3030,sp@-
	clrl d2
	lea _array,a0
	moveq #100,d3	<------ D3 now has limit of the test
	cmpl d2,d3
	jle L19
L2:
	moveq #3,d3	<----- D3 is being reused!!
	movl d3,a0@(d2:l:4)
	addql #1,d2
	moveq #100,d3	<----- D3 is being reloaded!
	cmpl d2,d3
	jgt L2
L19:
	clrl d2
	lea _darray,a2
	moveq #10,d3
	cmpl d2,d3
	jle L18
L7:
	clrl d1
	movl d2,d0
	asll #5,d0
	movl d0,a3
	lea a3@(d2:l:8),a1
	moveq #10,d3
	cmpl d1,d3
	jle L17
L10:
	lea a1@(d1:l:4),a0
	moveq #3,d3
	movl d3,a2@(a0:l)
	addql #1,d1
	moveq #10,d3
	cmpl d1,d3
	jgt L10
L17:
	addql #1,d2
	moveq #10,d3
	cmpl d2,d3
	jgt L7
L18:
	moveml a6@(-16),#0xc0c
	unlk a6
	rts

Alan Stebbens (aks@acc-sb-unix.ARPA)
*/
