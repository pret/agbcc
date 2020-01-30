/* CYGNUS LOCAL entire file/d30v */
/* GNU C stdarg/varargs support for the D30V */

/* Define __gnuc_va_list.  */
#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST
typedef struct __va_list_tag {
  int *__va_arg_ptr;		/* start of the register save area */
  int __va_arg_num;		/* argument number */
} __va_list[1], __gnuc_va_list[1];

#endif /* not __GNUC_VA_LIST */

/* If this is for internal libc use, don't define anything but
   __gnuc_va_list.  */
#if defined (_STDARG_H) || defined (_VARARGS_H)

/* Common code for va_start for both varargs and stdarg.  This depends
   on the format of the CUMULATIVE_ARGS type.  On the d30v, we use just
   a single word that is the register number.  */

#define __va_start_common(AP)						\
__extension__ ({							\
   (AP)->__va_arg_ptr = (int *) __builtin_saveregs ();			\
   (AP)->__va_arg_num = __builtin_args_info (0) - 2 /* first arg # */;	\
   (void)0;								\
})

#ifdef _STDARG_H /* stdarg.h support */

/* Calling __builtin_next_arg gives the proper error message if LASTARG is
   not indeed the last argument.  */
#define va_start(AP,LASTARG) \
  (__builtin_next_arg (LASTARG), __va_start_common (AP))

#else /* varargs.h support */

#define va_start(AP) __va_start_common (AP)
#define va_alist __builtin_va_alist
#define va_dcl register int va_alist; ...

#endif /* _STDARG_H */

/* Nothing needs to be done to end varargs/stdarg processing */
#define va_end(AP) ((void)0)

#define va_arg(AP,TYPE)							\
__extension__ (*({							\
  register TYPE *__ptr;							\
									\
  if (sizeof (TYPE) > 4 && ((AP)->__va_arg_num & 1) != 0)		\
    (AP)->__va_arg_num++;						\
									\
  __ptr = (TYPE *)(((char *)(void *)					\
		    ((AP)->__va_arg_ptr + (AP)->__va_arg_num)));	\
									\
  if (sizeof (TYPE) < 4)						\
    __ptr = (void*)__ptr + 4 - sizeof (TYPE);				\
  (AP)->__va_arg_num += (sizeof (TYPE) + 3) / 4;			\
  __ptr;								\
}))

#endif /* defined (_STDARG_H) || defined (_VARARGS_H) */
/* END CYGNUS LOCAL */
