/*
 * string.h
 *
 * Definitions for memory and string functions.
 */

#ifndef _STRING_H_
#define _STRING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/config.h>

#define __need_size_t
#include <stddef.h>

#ifndef NULL
#define NULL 0L
#endif

void *memchr(const void *, int, size_t);
int memcmp(const void *, const void *, size_t);
void *memcpy(void *, const void *, size_t);
void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);
char *strcat(char *, const char *);
char *strchr(const char *, int);
int strcmp(const char *, const char *);
int strcoll(const char *, const char *);
char *strcpy(char *, const char *);
size_t strcspn(const char *, const char *);
char *strerror(int);
size_t strlen(const char *);
char *strncat(char *, const char *, size_t);
int strncmp(const char *, const char *, size_t);
char *strncpy(char *, const char *, size_t);
char *strpbrk(const char *, const char *);
char *strrchr(const char *, int);
size_t strspn(const char *, const char *);
char *strstr(const char *, const char *);

#ifndef _REENT_ONLY
char *strtok(char *, const char *);
#endif

size_t strxfrm(char *, const char *, size_t);

#ifndef __STRICT_ANSI__
char *strtok_r(char *, const char *, char **);

int bcmp(const char *, const char *, size_t);
void bcopy(const char *, char *, size_t);
void bzero(char *, size_t);
int ffs(int);
char *index(const char *, int);
void *memccpy(void *, const void *, int, size_t);
char *rindex(const char *, int);
int strcasecmp(const char *, const char *);
char *strdup(const char *);
int strncasecmp(const char *, const char *, size_t);
char *strsep(char **, const char *);
char *strlwr(char *);
char *strupr(char *);
#ifdef __CYGWIN32__
char *strsignal(int __signo);
int strtosigno(const char *__name);
#endif

/* These function names are used on Windows and perhaps other systems.  */
#ifndef strcmpi
#define strcmpi strcasecmp
#endif
#ifndef stricmp
#define stricmp strcasecmp
#endif
#ifndef strncmpi
#define strncmpi strncasecmp
#endif
#ifndef strnicmp
#define strnicmp strncasecmp
#endif

#endif /* ! __STRICT_ANSI__ */

#ifdef __cplusplus
}
#endif
#endif /* _STRING_H_ */
