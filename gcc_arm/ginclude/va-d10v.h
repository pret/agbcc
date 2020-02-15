/* GNU C stdarg/varargs support for the D10V */

/* Define __gnuc_va_list.  */
#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST
typedef struct __va_list_tag {
  short *__va_reg_base;		/* start of the register save area */
  short __va_reg_num;		/* argument number */
  char *__va_stack;
} __va_list[1], __gnuc_va_list[1];

#endif /* not __GNUC_VA_LIST */

/* If this is for internal libc use, don't define anything but
   __gnuc_va_list.  */
#if defined (_STDARG_H) || defined (_VARARGS_H)

/* Common code for va_start for both varargs and stdarg.  This depends
   on the format of the CUMULATIVE_ARGS type.  On the d10v, we use just
   a single word that is the argument number.  */

#define __va_start_common(AP)						\
__extension__ ({							\
   (AP)->__va_reg_base = (short *) __builtin_saveregs ();		\
   (AP)->__va_reg_num = __builtin_args_info (0);			\
   (AP)->__va_stack							\
     = (char *)((AP)->__va_reg_base + __builtin_args_info (1));		\
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
  int __va_reg_now = (AP)->__va_reg_num, __va_reg_new;			\
									\
  if (sizeof (TYPE) >= 4 && (__va_reg_now & 1) != 0)			\
    __va_reg_now++;							\
  __va_reg_new = __va_reg_now + (sizeof (TYPE) + 1) / 2;		\
  if (__va_reg_new <= 4)						\
    {									\
      (AP)->__va_reg_num = __va_reg_new;				\
      __ptr = (TYPE *)(((char *)(void *)				\
			((AP)->__va_reg_base + __va_reg_now))		\
		       + (sizeof (TYPE) < 2));				\
    }									\
  else									\
    {									\
      /* ??? According to PARM_BOUNDARY, there should be no extra	\
	 alignment here - but there is, see testcase execute/va-arg-6.c.\
	 That seems to be a backend bug */				\
      if (sizeof (TYPE) >= 4						\
	  && (((AP)->__va_stack - (char *)(AP)->__va_reg_base) & 2) != 0)\
									\
	(AP)->__va_stack += 2;						\
      __ptr = (TYPE *)((AP)->__va_stack	+ (sizeof (TYPE) < 2));		\
      (AP)->__va_stack += (sizeof (TYPE) + 1) & ~1;			\
    }									\
									\
  __ptr;								\
}))

#endif /* defined (_STDARG_H) || defined (_VARARGS_H) */
