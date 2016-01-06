/*
 * special support for simulator
 *
 *   Copyright (C) 1996 Free Software Foundation, Inc.
 *   Written By Michael Meissner
 * 
 * This file is free software * you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation * either version 2, or (at your option) any
 * later version.
 * 
 * In addition to the permissions in the GNU General Public License, the
 * Free Software Foundation gives you unlimited permission to link the
 * compiled version of this file with other programs, and to distribute
 * those programs without any restriction coming from the use of this
 * file.  (The General Public License restrictions do apply in other
 * respects * for example, they cover modification of the file, and
 * distribution when not linked into another program.)
 * 
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY * without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program * see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 * 
 *    As a special exception, if you link this library with files
 *    compiled with GCC to produce an executable, this does not cause
 *    the resulting executable to be covered by the GNU General Public License.
 *    This exception does not however invalidate any other reasons why
 *    the executable file might be covered by the GNU General Public License.
 */

	.file	"sim.s"
	.text

	.globl	printf
	.type	printf,@function
printf:
	trap	2
	jmp	r13
	.size	printf,.-printf

	.globl	putchar
	.type	putchar,@function
putchar:
	trap	3
	jmp	r13
	.size	putchar,.-putchar

	.globl	putstr
	.type	putstr,@function
putstr:
	trap	1
	jmp	r13
	.size	putstr,.-putstr

	.globl	exit
	.type	exit,@function
	.globl	_exit
	.type	_exit,@function
exit:
_exit:
	ldi	r6,1 -> trap 0			/* SYS_exit */
	stop
	.size	exit,.-exit
	.size	_exit,.-_exit

	.globl	abort
	.type	aobrt,@function
abort:
	ldi	r2,amsg
	trap	1
	ldi	r6,1 || ldi r2,1		/* SYS_exit & non-zero return code */
	trap	0 -> stop

	.section .rodata
amsg:	.ascii	"Abort called\n\0"
