/*
Return-Path: <ucsfcgl!reagan.ai.mit.edu!RUTGERS.EDU!RUTGERS.EDU!uiucdcs!uiucuxc!sys1!sneaky!gordon>
Date: Tue Dec 15, 1987 at 10:25:42 cst
From: uiucdcs!uiucuxc!sys1!sneaky!gordon@rutgers.edu (Gordon Burditt)
Message-Id: <8712151625.AA15898@sneaky.UUCP>
To: uiucdcs!uiucuxc!sys1!techsup!vaxnix!ron@rutgers.edu,
        bug-gcc@prep.ai.mit.edu
Subject: gcc bug
Sent: Tue Dec 15, 1987 at 10:25:42 cst

I have been asked to pass along this bug report for gcc 1.15.
I am having trouble figuring out from the ANSI spec whether this really
is a bug or not.  ANSI says that side effects caused by function argument
evaluation must be complete by the time the call is done, but how about side 
effects in evaluating the function address?

From: ron@vaxnix.tandy.COM (Ron Light)
Date: Sat, 12 Dec 87 7:50:53 CST
*/

typedef int (*Inst)();		/* machine instruction */
#define STOP (Inst)(0)

Inst *pc;			/* program counter during execution */

execute(p)			/* run the machine */
Inst *p;
{
	for(pc = p; *pc != STOP;)
		(*(*pc++))();
}

int inst();
Inst instrs[] = {inst, inst, inst, STOP};
int inst() {
	if (pc == &instrs[1])
		printf("Test passed\n");
	else
		printf("FAILED, pc = %x, instrs = %x\n", pc, instrs);
	exit(0);
}
main()
{

	execute(instrs);
}
/*
code produced by gcc:
.text
	.even
.globl _execute
_execute:
	link a6,#0
	movl a6@(8),_pc
	jra L2
L5:
	movl _pc,a0
	movl a0@,a0        ;<---------------------------+
	jbsr a0@           ;                            |
	addql #4,_pc       ; This should be here--------+
L2:
	movl _pc,a0
	tstl a0@
	jne L5
	unlk a6
	rts
.comm _pc,4

*/
