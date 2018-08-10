/*
 * stdlib.h
 *
 * Definitions for common types, variables, and functions.
 */

#ifndef _STDLIB_H_
#ifdef __cplusplus
extern "C" {
#endif
#define _STDLIB_H_

#include <sys/config.h>

#define __need_size_t
#define __need_wchar_t
#include <stddef.h>

#include <sys/reent.h>

typedef struct
{
    int quot; /* quotient */
    int rem;  /* remainder */
} div_t;

typedef struct
{
    long quot; /* quotient */
    long rem;  /* remainder */
} ldiv_t;

#ifndef NULL
#define NULL 0L
#endif

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define RAND_MAX 0x7fffffff

#if (defined(__CYGWIN__) || defined(__CYGWIN32__)) && !defined(_COMPILING_NEWLIB)
extern __declspec(dllimport) int __mb_cur_max;
#else
extern int __mb_cur_max;
#endif

#define MB_CUR_MAX __mb_cur_max

void abort(void) __attribute__((noreturn));
int abs(int);
int atexit(void (*__func)(void));
double atof(const char *__nptr);
#ifndef __STRICT_ANSI__
float atoff(const char *__nptr);
#endif
int atoi(const char *__nptr);
long atol(const char *__nptr);
void *bsearch(const void *__key, const void *__base, size_t __nmemb, size_t __size,
    int (*_compar)(const void *, const void *));
void *calloc(size_t __nmemb, size_t __size);
div_t div(int __numer, int __denom);
void exit(int __status) __attribute__((noreturn));
void free(void *);
char *getenv(const char *__string);
long labs(long);
ldiv_t ldiv(long __numer, long __denom);
void *malloc(size_t __size);
int mblen(const char *, size_t);
int mbtowc(wchar_t *, const char *, size_t);
int _mbtowc_r(struct _reent *, wchar_t *, const char *, size_t, int *);
int wctomb(char *, wchar_t);
int _wctomb_r(struct _reent *, char *, wchar_t, int *);
size_t mbstowcs(wchar_t *, const char *, size_t);
size_t _mbstowcs_r(struct _reent *, wchar_t *, const char *, size_t, int *);
size_t wcstombs(char *, const wchar_t *, size_t);
size_t _wcstombs_r(struct _reent *, char *, const wchar_t *, size_t, int *);
void qsort(void *__base, size_t __nmemb, size_t __size, int (*_compar)(const void *, const void *));
int rand(void);
void *realloc(void *__r, size_t __size);
void srand(unsigned __seed);
double strtod(const char *__n, char **_end_PTR);
#ifndef __STRICT_ANSI__
float strtodf(const char *__n, char **_end_PTR);
#endif
long strtol(const char *__n, char **_end_PTR, int __base);
unsigned long strtoul(const char *_n_PTR, char **_end_PTR, int __base);
unsigned long _strtoul_r(struct _reent *, const char *_n_PTR, char **_end_PTR, int __base);
int system(const char *__string);

#ifndef __STRICT_ANSI__
void cfree(void *);
int putenv(const char *__string);
int setenv(const char *__string, const char *__value, int __overwrite);

char *gcvt(double, int, char *);
char *gcvtf(float, int, char *);
char *fcvt(double, int, int *, int *);
char *fcvtf(float, int, int *, int *);
char *ecvt(double, int, int *, int *);
char *ecvtbuf(double, int, int *, int *, char *);
char *fcvtbuf(double, int, int *, int *, char *);
char *ecvtf(float, int, int *, int *);
char *dtoa(double, int, int, int *, int *, char **);
int rand_r(unsigned *__seed);

#ifdef __CYGWIN32__
char *realpath(const char *, char *);
void unsetenv(const char *__string);
int random(void);
long srandom(unsigned __seed);
#endif

#endif /* ! __STRICT_ANSI__ */

char *_dtoa_r(struct _reent *, double, int, int, int *, int *, char **);
void *_malloc_r(struct _reent *, size_t);
void *_calloc_r(struct _reent *, size_t, size_t);
void _free_r(struct _reent *, void *);
void *_realloc_r(struct _reent *, void *, size_t);
void _mstats_r(struct _reent *, char *);
int _system_r(struct _reent *, const char *);

void __eprintf(const char *, const char *, unsigned int, const char *);

#ifdef __cplusplus
}
#endif
#endif /* _STDLIB_H_ */
