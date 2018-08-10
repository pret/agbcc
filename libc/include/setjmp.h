/*
    setjmp.h
    stubs for future use.
*/

#ifndef _SETJMP_H_
#ifdef __cplusplus
extern "C" {
#endif
#define _SETJMP_H_

#include <sys/config.h>
#include <machine/setjmp.h>

void longjmp(jmp_buf __jmpb, int __retval);
int setjmp(jmp_buf __jmpb);

#ifdef __cplusplus
}
#endif
#endif /* _SETJMP_H_ */
