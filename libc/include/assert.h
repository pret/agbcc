/*
    assert.h
*/

#ifdef __cplusplus
extern "C" {
#endif

#undef assert

#ifdef NDEBUG /* required by ANSI standard */
#define assert(p) ((void)0)
#else

#define assert(e) ((e) ? (void)0 : __assert(__FILE__, __LINE__, #e))

#endif /* NDEBUG */

void __assert(const char *, int, const char *);

#ifdef __cplusplus
}
#endif
