/*
Message-Id: <8901111052.AA21004@hop.toad.com>
To: rms
Cc: gnu
Subject: gcc-1.32 sparc bug: uses invalid addressing mode
Date: Wed, 11 Jan 89 02:52:19 -0800
From: gnu

The following program crashes because

	sth	...,[_foo]

is not valid, since it only has a few displacement bits but needs to represent
a 32-bit address.  Unfortunately the Sun assembler does not catch this at
assembly time, nor does the linker complain.

This is excerpted from gnuchess.

I tried to track it down myself; things are OK through the lreg pass,
which marks this pseudo-reg as live in "-1" instrs, indicating that
it is infrequently used and should not get a register.  Greg deletes
the instruction that loads it, and substitutes the name _Developed
for the pseudo-reg, apparently not realizing that this is invalid.

	John

[In gcc-1.33 prototype, this coredumps cc1 in 
#0  0x77978 in next_insn_tests_no_inequality (insn=(rtx) 0xa6750) (recog.c line 97)
#1  0x60858 in subst (x=(rtx) 0xa66a0, to=(rtx) 0xa6630, from=(rtx) 0xa6628) (combine.c line 1063)
#2  0x5fd44 in subst (x=(rtx) 0xa6728, to=(rtx) 0xa6630, from=(rtx) 0xa6628) (combine.c line 852)
#3  0x5fd44 in subst (x=(rtx) 0xa6880, to=(rtx) 0xa6630, from=(rtx) 0xa6628) (combine.c line 852)
#4  0x5fd44 in subst (x=(rtx) 0xa6740, to=(rtx) 0xa6630, from=(rtx) 0xa6628) (combine.c line 852)
#5  0x5f3c0 in try_combine (i3=(rtx) 0xa6750, i1=(rtx) 0xa6648, i2=(rtx) 0xa66c0) (combine.c line 521)
#6  0x5eb58 in combine_instructions (f=(rtx) 0xa6828, nregs=276) (combine.c line 272)
#7  0x1ad1c in rest_of_compilation (decl=(tree) 0xa8cf0) (toplev.c line 1357)
#8  0x10cb8 in finish_function () (c-decl.c line 3507)
#9  0x5d7c in yyparse () (c-parse.y line 244)
#10 0x19ed8 in compile_file (name=(char *) 0xf7fffa67 "foo.c") (toplev.c line 966)
#11 0x1bbb4 in main (argv=(char **) 0xf7fff98c, argc=5) (toplev.c line 1709)

(gdb) p *insn
$1 = {code = INSN, mode = VOIDmode, jump = 0, call = 0, unchanging = 0, volatil = 0, in_struct = 0, used = 0, integrated = 0, fld = {{rtint = 8, rtstr = 0x8 <Address 0x8 out of bounds>, rtx = 0x8, rtvec = 0x8, rttype = SFmode}}}
(gdb) 

I am wondering why "nregs" in combine_instructions (#6) is so high on such a
small program.  This may have nothing to do with the bug.

	John
]

*/


short board[64];
short Developed[1];

ExaminePosition()
{
    Developed[0] = board[0] != 2;
}



/* main() added for test purposes. */
main()
{
	int fail = 0;

	board[0] = 5;
	ExaminePosition();
	if (Developed[0] != 1) {
		printf("Test #1 failed, Developed[0] = %d\n", Developed[0]);
		fail++;
	}
	board[0] = 2;
	ExaminePosition();
	if (Developed[0] != 0) {
		printf("Test #2 failed, Developed[0] = %d\n", Developed[0]);
		fail++;
	}
	if (fail == 0)
		printf("Test passed\n");
}
