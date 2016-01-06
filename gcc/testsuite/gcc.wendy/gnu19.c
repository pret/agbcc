/*
Date: Wed, 7 Oct 87 11:54:57 PDT
From: faustus%ic.Berkeley.EDU@cad.Berkeley.EDU (Wayne A. Christopher)
To: bug-gcc@PREP.AI.MIT.EDU
Subject: GNU CC bug

The following fragment of code causes the message "xx.c:7: parse error" to
be printed:
*/
	typedef struct foo {
		int i;
	} foo;

	typedef struct bar {
		foo *foo;
	} bar;

main(){
	printf("Test passed (if it compiled)\n");
}
/*
I can't find anything in the ANSI standard saying that a structure tag can't
be the same as a typedef name.

	Wayne
*/
