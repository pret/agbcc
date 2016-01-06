/*
Return-Path: <ucsfcgl!TOR.NTA.NO!H_Eidnes%vax.runit.unit.uninett>
Date: Tue, 23 Jun 87 17:17:38 +0200
From: H}vard Eidnes <ucsfcgl!TOR.NTA.NO!H_Eidnes%vax.runit.unit.uninett>
To: <bug-gcc@prep.ai.mit.edu>
Message-Id: <2178:H_Eidnes@vax.runit.unit.uninett>
Subject: Spurious error-message (?) from GCC

The following is an exerpt from a header file from the X Window
System, version 10r3.

GCC (version 1.5) says:
/usr/include/X/Xlib.h:108: declaration of table invalidly omits array size
*/

typedef struct _x_assoc_table {
	struct _x_assoc (*table)[];	/* Array of association queues. */
	int size;			/* Table size. */
} XAssocTable;

main() {
	printf("Test passed\n");
}
