/* Assembly support functions for libgcc1.
 *
 *   Copyright (C) 1996, 1997, 1998 Free Software Foundation, Inc.
 *   Contributed by Cygnus Support
 * 
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 * 
 * In addition to the permissions in the GNU General Public License, the
 * Free Software Foundation gives you unlimited permission to link the
 * compiled version of this file with other programs, and to distribute
 * those programs without any restriction coming from the use of this
 * file.  (The General Public License restrictions do apply in other
 * respects; for example, they cover modification of the file, and
 * distribution when not linked into another program.)
 * 
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 * 
 *    As a special exception, if you link this library with files
 *    compiled with GCC to produce an executable, this does not cause
 *    the resulting executable to be covered by the GNU General Public License.
 *    This exception does not however invalidate any other reasons why
 *    the executable file might be covered by the GNU General Public License.
 */


#ifdef L_ashlsi3
/* Shift of 32 bit int left, r0-r1 == value to be shifted, r2 = shift amount */	

	.file	"_ashlsi3.s"
	.text
	.globl	__ashlsi3
	.type	__ashlsi3,@function
__ashlsi3:
	mv2wfac r4,a0				/* preserve accumulator & guard digits */
	mvfacg	r12,a0
	mv2wtac r0,a0
	cmpui	r2,16
	and3	r0,r2,15
	exef0f || slli a0,16
	sll	a0,r0
	mv2wfac r0,a0
	mv2wtac	r4,a0
	mvtacg	r12,a0
	jmp	r13
	.size	__ashlsi3,.-__ashlsi3
#endif

#ifdef L_ashrsi3
/* Shift of 32 bit signed int right, r0-r1 == value to be shifted, r2 = shift amount */	

	.file	"_ashrsi3.s"
	.text
	.globl	__ashrsi3
	.type	__ashrsi3,@function
__ashrsi3:
	mv2wfac r4,a0				/* preserve accumulator & guard digits */
	mvfacg	r12,a0
	mv2wtac r0,a0
	cmpui	r2,16
	and3	r0,r2,15
	exef0f || srai a0,16
	sra	a0,r0
	mv2wfac r0,a0
	mv2wtac	r4,a0
	mvtacg	r12,a0
	jmp	r13
	.size	__ashrsi3,.-__ashrsi3
#endif

#ifdef L_lshrsi3
/* Shift of 32 bit unsigned int right, r0-r1 == value to be shifted, r2 = shift amount */	

	.file	"_lshrsi3.s"
	.text
	.globl	__lshrsi3
	.type	__lshrsi3,@function
__lshrsi3:
	mv2wfac r4,a0				/* preserve accumulator & guard digits */
	mvfacg	r12,a0
	mv2wtac r0,a0
	mvtacg	r14,a0				/* zero guard digits */
	cmpui	r2,16
	and3	r0,r2,15
	exef0f || srli a0,16
	srl	a0,r0
	mv2wfac r0,a0
	mv2wtac	r4,a0
	mvtacg	r12,a0
	jmp	r13
	.size	__lshrsi3,.-__lshrsi3
#endif

#ifdef L_mulsi3
/* Multiply 2 32-bit signed integers with a 32-bit result, r0-r1 = first number,
   r2-r3 = second number.  */

	.file	"_mulsi3.s"
	.text
	.globl	__mulsi3
	.type	__mulsi3,@function
__mulsi3:
	mv2wfac r4,a0				/* preserve accumulator & guard digits */
	mvfacg	r12,a0
	mulxu	a0,r0,r3
	macu	a0,r1,r2
	slli	a0,16
	macu	a0,r1,r3
	mv2wfac r0,a0
	mv2wtac	r4,a0
	mvtacg	r12,a0
	jmp	r13
	.size	__mulsi3,.-__mulsi3
#endif


#ifdef L_udivmodhi4
/* Unsigned divide 2 16-bit numbers, leaving the remainder in r0, and quotient
   in r1.  */

	.file	"_udivmodhi4.s"
	.text
	.globl	__udivmodhi4
	.type	__udivmodhi4,@function
	.globl	__umodhi3
	.type	__umodhi3,@function
__udivmodhi4:
__umodhi3:
	mv	r2,r1 -> mv r1,r0	/* move divisor to safe register & create 32-bit value for dividend */
	ldi	r0,0			/* high part of dividend */
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	jmp	r13
	.size	__udivmodhi4,.-__udivmodhi4
	.size	__umodhi3,.-__umodhi3

	.globl	__udivhi3
	.type	__udivhi3,@function
__udivhi3:
	st	r13,@-sp
	bl	__udivmodhi4
	mv	r0,r1
	ld	r13,@sp+
	jmp	r13
	.size	__udivhi3,.-__udivhi3
#endif


#ifdef L_divmodhi4
/* Signed divide 2 16-bit numbers, leaving the remainder in r0, and quotient
   in r1.  */

	.file	"_divmodhi4.s"
	.text
	.globl	__divmodhi4
	.type	__divmodhi4,@function
	.globl	__modhi3
	.type	__modhi3,@function
__divmodhi4:
__modhi3:
	mv	r2,r1	-> mv r1,r0	/* move divisor to safe register & create 32-bit value for dividend */
	ldi	r0,0	|| abs r1	/* high part of dividend */
	abs	r2	|| nop		/* make sure divisor & dividend are positive (F0/F1 indicate if args were negative) */
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	divs	r0,r2
	exef1t || neg r0
	exef0t || neg r1
	exef1t || neg r1
	jmp	r13
	.size	__divmodhi4,.-__divmodhi4
	.size	__modhi3,.-__modhi3

	.globl	__divhi3
	.type	__divhi3,@function
__divhi3:
	st	r13,@-sp
	bl	__divmodhi4
	mv	r0,r1
	ld	r13,@sp+
	jmp	r13
	.size	__divhi3,.-__divhi3
#endif


#ifdef L_udivmodsi4
/* Unsigned divide 2 32-bit numbers, leaving the remainder in r0-r1, and quotient
   in r2-r3.  */

	.file	"_udivmodsi4.s"
	.text
	.globl	__udivmodsi4
	.type	__udivmodsi4,@function
	.globl	__udivsi3
	.type	__udivsi3,@function
	/* input:	r0-r1 = dividend, r2-r3 = divisor */
	/* output:	r0-r1 = quotient, r2-r3 = remainder */
__udivmodsi4:
__udivsi3:
	cmpeqi	r0,0 ->	brf0f.s	.Llong	/* if we are doing short/short do it with divs */
	cmpeqi	r2,0 -> brf0f.s	.Llong

	divs	r0,r3
	divs	r0,r3
	divs	r0,r3
	divs	r0,r3
	divs	r0,r3
	divs	r0,r3
	divs	r0,r3
	divs	r0,r3
	divs	r0,r3
	divs	r0,r3
	divs	r0,r3
	divs	r0,r3
	divs	r0,r3
	divs	r0,r3
	divs	r0,r3
	divs	r0,r3
	mv	r3,r0
	ldi	r0,0
	jmp	r13

.Llong:
	st2w	r6,@-sp
	st2w	r8,@-sp
	mv2w	r4,r2			/* divisor (b) */
	ldi	r2,0			/* high part of dividend (ah) */
	ldi	r3,0
	ldi	r12,32			/* number of iterations */
.Lloop:
	mv2w	r6,r2			/* tmp1 = (ah << 1) | (al >> 31) */
	add2w	r6,r6
	cmpi	r0,0
	exef0t || addi r7,1

	mv2w	r8,r6			/* tmp2 = tmp1 - b */
	sub2w	r8,r4

	cmpeq	r6,r4			/* tmpf = (tmp1 >= b), ie f0 = !(tmp1 < b) */
	cmpu	r6,r4
	exef1t || cmpu r7,r5

	mv2w	r2,r6			/* ah = (tmpf) ? tmp1 : tmp2 */
	exef0f || mv2w r2,r8

	add2w	r0,r0			/* al = (al << 1) | tmpf */
	exef0f || addi r1,1

	subi	r12,1			/* decrement and loop */
	cmpeqi	r12,0
	brf0f.s	.Lloop			/* work around simulator bug with .s version of brf0f */

	ld2w	r8,@sp+
	ld2w	r6,@sp+
	jmp	r13
	.size	__udivmodsi4,.-__udivmodsi4
	.size	__udivsi3,.-__udivsi3

	.globl	__umodsi3
	.type	__umodsi3,@function
__umodsi3:
	st	r13,@-sp
	bl	__udivmodsi4
	mv2w	r0,r2
	ld	r13,@sp+
	jmp	r13
	.size	__modvsi3,.-__modvsi3
#endif


#ifdef L_divmodsi4
/* Signed divide 2 32-bit numbers, leaving the remainder in r0-r1, and quotient
   in r2-r3.  */

	.file	"_divmodsi4.s"
	.text
	.globl	__divmodsi4
	.type	__divmodsi4,@function
	.globl	__divsi3
	.type	__divsi3,@function
__divmodsi4:
__divsi3:
	cmpi	r0,0				/* divisor negative? */
	brf0t.s	.Lneg1

	cmpi	r2,0				/* dividend negative? */
	brf0t.s	.Lneg3

	bra	__udivmodsi4			/* both positive, just do division, __udivmodsi4 will return to caller */

.Lneg1:						/* divisor negative, dividend sign not yet checked */
	st	r13,@-sp
	not	r0				/* negate divisor */
	neg	r1
	cmpeqi	r1,0
	exef0t || addi r0,1

	cmpi	r2,0				/* dividend negative? */
	brf0t.s	.Lneg2

	bl	__udivmodsi4			/* do division */

	not	r0				/* negate quotient */
	neg	r1
	cmpeqi	r1,0
	exef0t || addi r0,1

	not	r2				/* negate remainder */
	neg	r3
	cmpeqi	r3,0
	exef0t || addi r2,1
	ld	r13,@sp+
	jmp	r13

.Lneg2:						/* divisor & dividend negative */
	not	r2				/* negate dividend */
	neg	r3
	cmpeqi	r3,0
	exef0t || addi r2,1

	bl	__udivmodsi4			/* do division */

	not	r2				/* negate remainder */
	neg	r3
	cmpeqi	r3,0
	exef0t || addi r2,1
	ld	r13,@sp+
	jmp	r13

.Lneg3:						/* divisor positive, dividend negative */
	st	r13,@-sp
	not	r2				/* negate dividend */
	neg	r3
	cmpeqi	r3,0
	exef0t || addi r2,1

	bl	__udivmodsi4			/* do division */	

	not	r0				/* negate quotient */
	neg	r1
	cmpeqi	r1,0
	exef0t || addi r0,1
	ld	r13,@sp+
	jmp	r13
	.size	__divmodsi4,.-__divmodsi4
	.size	__divsi3,.-__divsi3

	.globl	__modsi3
	.type	__modsi3,@function
__modsi3:
	st	r13,@-sp
	bl	__divmodsi4
	mv2w	r0,r2
	ld	r13,@sp+
	jmp	r13
	.size	__modsi3,.-__modsi3
#endif


#ifdef L_cmpdi
/* Signed compare 2 64-bit ints */

	.file	"__cmpdi.s"
	.text
	.globl	__cmpdi
	.type	__cmpdi,@function
	/* input:	r0-r3 is first value to compare, second value to compare on stack */
	/* output:	r0 is 0 if op0 < op1, 1 if op0 == op1, 2 if op0 > op1 */
__cmpdi:
	ld2w	r4,@sp			/* compare first word */
	cmp	r0,r4
	brf0t.s	.Lneg
	cmp	r4,r0
	brf0t.s	.Lpos

	cmpu	r1,r5			/* compare second word */
	brf0t.s	.Lneg
	cmpu	r5,r1
	brf0t.s	.Lpos
	
	ld2w	r4,@(4,sp)		/* compare third word */
	cmpu	r2,r4
	brf0t.s	.Lneg
	cmpu	r4,r2
	brf0t.s	.Lpos
	
	cmpu	r3,r5			/* compare fourth word */
	brf0t.s	.Lneg
	cmpu	r5,r3
	brf0t.s	.Lpos

	ldi	r0,1			/* op1 == op2 */
	jmp	r13

.Lneg:
	ldi	r0,0			/* op1 < op2 */
	jmp	r13

.Lpos:
	ldi	r0,2			/* op1 > op2 */
	jmp	r13
	.size	__cmpdi,.-__cmpdi
#endif


#ifdef L_cmpdi0
/* Signed compare 64-bit int against 0 */

	.file	"__cmpdi0.s"
	.text
	.globl	__cmpdi0
	.type	__cmpdi0,@function
	/* input:	r0-r3 is value to compare */
	/* output:	r0 is -1 if op0 < 0, 0 if op0 == 0, 1 if op0 > 0 */
__cmpdi0:
	cmpi	r0,0			/* compare first word */
	brf0t.s	.Lneg
	cmpeqi	r0,0
	brf0f.s	.Lpos

	cmpeqi	r1,0			/* compare second word */
	brf0f.s	.Lpos

	cmpeqi	r2,0			/* compare third word */
	brf0f.s	.Lpos

	cmpeqi	r3,0			/* compare fourth word */
	brf0f.s	.Lpos
	
	ldi	r0,1			/* op1 == 0 */
	jmp	r13

.Lneg:
	ldi	r0,0			/* op1 < 0 */
	jmp	r13

.Lpos:
	ldi	r0,2			/* op1 > 0 */
	jmp	r13
	.size	__cmpdi0,.-__cmpdi0
#endif


#ifdef L_ucmpdi
/* Unsigned compare 2 64-bit ints */

	.file	"__ucmpdi.s"
	.text
	.globl	__ucmpdi
	.type	__ucmpdi,@function
	/* input:	r0-r3 is first value to compare, second value to compare on stack */
	/* output:	r0 is -1 if (unsigned)op0 < (unsigned)op1, 0 if op0 == op1, 1 if (unsigned)op0 > (unsigned)op1 */
__ucmpdi:
	ld2w	r4,@sp			/* compare first word */
	cmpu	r0,r4
	brf0t.s	.Lneg
	cmpu	r4,r0
	brf0t.s	.Lpos

	cmpu	r1,r5			/* compare second word */
	brf0t.s	.Lneg
	cmpu	r5,r1
	brf0t.s	.Lpos
	
	ld2w	r4,@(4,sp)		/* compare third word */
	cmpu	r2,r4
	brf0t.s	.Lneg
	cmpu	r4,r2
	brf0t.s	.Lpos
	
	cmpu	r3,r5			/* compare fourth word */
	brf0t.s	.Lneg
	cmpu	r5,r3
	brf0t.s	.Lpos

	ldi	r0,1			/* op1 == op2 */
	jmp	r13

.Lneg:
	ldi	r0,0			/* op1 < op2 */
	jmp	r13

.Lpos:
	ldi	r0,2			/* op1 > op2 */
	jmp	r13
	.size	__ucmpdi,.-__ucmpdi
#endif


#ifdef L_ucmpdi0
/* Unsigned compare 64-bit int against 0 */

	.file	"__ucmpdi0.s"
	.text
	.globl	__ucmpdi0
	.type	__ucmpdi0,@function
	/* input:	r0-r3 is value to compare */
	/* output:	r0 is 0 if op0 == 0, 1 if op0 > 0 */
__ucmpdi0:
	cmpeqi	r0,0			/* compare first word */
	brf0f.s	.Lpos

	cmpeqi	r1,0			/* compare second word */
	brf0f.s	.Lpos

	cmpeqi	r2,0			/* compare third word */
	brf0f.s	.Lpos

	cmpeqi	r3,0			/* compare fourth word */
	brf0f.s	.Lpos
	
	ldi	r0,1			/* op1 == 0 */
	jmp	r13

.Lpos:
	ldi	r0,2			/* op1 > 0 */
	jmp	r13
	.size	__ucmpdi0,.-__ucmpdi0
#endif
