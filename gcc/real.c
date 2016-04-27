/* real.c - implementation of REAL_ARITHMETIC, REAL_VALUE_ATOF,
   and support for XFmode IEEE extended real floating point arithmetic.
   Copyright (C) 1993, 94-98, 1999 Free Software Foundation, Inc.
   Contributed by Stephen L. Moshier (moshier@world.std.com).

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "tree.h"
#include "toplev.h"

/* To enable support of XFmode extended real floating point, define
LONG_DOUBLE_TYPE_SIZE 96 in the tm.h file (m68k.h or i386.h).

To support cross compilation between IEEE, VAX and IBM floating
point formats, define REAL_ARITHMETIC in the tm.h file.

In either case the machine files (tm.h) must not contain any code
that tries to use host floating point arithmetic to convert
REAL_VALUE_TYPEs from `double' to `float', pass them to fprintf,
etc.  In cross-compile situations a REAL_VALUE_TYPE may not
be intelligible to the host computer's native arithmetic.

The emulator defaults to the host's floating point format so that
its decimal conversion functions can be used if desired (see
real.h).

The first part of this file interfaces gcc to a floating point
arithmetic suite that was not written with gcc in mind.  Avoid
changing the low-level arithmetic routines unless you have suitable
test programs available.  A special version of the PARANOIA floating
point arithmetic tester, modified for this purpose, can be found on
usc.edu: /pub/C-numanal/ieeetest.zoo.  Other tests, and libraries of
XFmode and TFmode transcendental functions, can be obtained by ftp from
netlib.att.com: netlib/cephes.   */

/* Type of computer arithmetic.
   Only one of DEC, IBM, IEEE, C4X, or UNK should get defined.

   `IEEE', when REAL_WORDS_BIG_ENDIAN is non-zero, refers generically
   to big-endian IEEE floating-point data structure.  This definition
   should work in SFmode `float' type and DFmode `double' type on
   virtually all big-endian IEEE machines.  If LONG_DOUBLE_TYPE_SIZE
   has been defined to be 96, then IEEE also invokes the particular
   XFmode (`long double' type) data structure used by the Motorola
   680x0 series processors.

   `IEEE', when REAL_WORDS_BIG_ENDIAN is zero, refers generally to
   little-endian IEEE machines. In this case, if LONG_DOUBLE_TYPE_SIZE
   has been defined to be 96, then IEEE also invokes the particular
   XFmode `long double' data structure used by the Intel 80x86 series
   processors.

   `DEC' refers specifically to the Digital Equipment Corp PDP-11
   and VAX floating point data structure.  This model currently
   supports no type wider than DFmode.

   `IBM' refers specifically to the IBM System/370 and compatible
   floating point data structure.  This model currently supports
   no type wider than DFmode.  The IBM conversions were contributed by
   frank@atom.ansto.gov.au (Frank Crawford).

   `C4X' refers specifically to the floating point format used on
   Texas Instruments TMS320C3x and TMS320C4x digital signal
   processors.  This supports QFmode (32-bit float, double) and HFmode
   (40-bit long double) where BITS_PER_BYTE is 32. Unlike IEEE
   floats, C4x floats are not rounded to be even. The C4x conversions
   were contributed by m.hayes@elec.canterbury.ac.nz (Michael Hayes) and
   Haj.Ten.Brugge@net.HCC.nl (Herman ten Brugge).

   If LONG_DOUBLE_TYPE_SIZE = 64 (the default, unless tm.h defines it)
   then `long double' and `double' are both implemented, but they
   both mean DFmode.  In this case, the software floating-point
   support available here is activated by writing
      #define REAL_ARITHMETIC
   in tm.h.

   The case LONG_DOUBLE_TYPE_SIZE = 128 activates TFmode support
   and may deactivate XFmode since `long double' is used to refer
   to both modes.

   The macros FLOAT_WORDS_BIG_ENDIAN, HOST_FLOAT_WORDS_BIG_ENDIAN,
   contributed by Richard Earnshaw <Richard.Earnshaw@cl.cam.ac.uk>,
   separate the floating point unit's endian-ness from that of
   the integer addressing.  This permits one to define a big-endian
   FPU on a little-endian machine (e.g., ARM).  An extension to
   BYTES_BIG_ENDIAN may be required for some machines in the future.
   These optional macros may be defined in tm.h.  In real.h, they
   default to WORDS_BIG_ENDIAN, etc., so there is no need to define
   them for any normal host or target machine on which the floats
   and the integers have the same endian-ness.   */


/* The following converts gcc macros into the ones used by this file.  */

/* REAL_ARITHMETIC defined means that macros in real.h are
   defined to call emulator functions.  */

#define IEEE

#define REAL_WORDS_BIG_ENDIAN FLOAT_WORDS_BIG_ENDIAN


/* Define INFINITY for support of infinity.
   Define NANS for support of Not-a-Number's (NaN's).  */
#define INFINITY
#define NANS

/* Construct macros to translate between REAL_VALUE_TYPE and e type.
   In GET_REAL and PUT_REAL, r and e are pointers.
   A REAL_VALUE_TYPE is guaranteed to occupy contiguous locations
   in memory, with no holes.  */

#define NE 6
#define MAXDECEXP 4932
#define MINDECEXP -4956
/* Emulator uses target format internally
   but host stores it in host endian-ness.  */

#define GET_REAL(r,e)						\
do {								\
     if (HOST_FLOAT_WORDS_BIG_ENDIAN == REAL_WORDS_BIG_ENDIAN)	\
       e53toe ((uint16_t *) (r), (e));			\
     else							\
       {							\
	 uint16_t w[4];					\
         memcpy (&w[3], ((int16_t *) r), sizeof (int16_t));		\
         memcpy (&w[2], ((int16_t *) r) + 1, sizeof (int16_t));	\
         memcpy (&w[1], ((int16_t *) r) + 2, sizeof (int16_t));	\
         memcpy (&w[0], ((int16_t *) r) + 3, sizeof (int16_t));	\
	 e53toe (w, (e));					\
       }							\
   } while (0)

#define PUT_REAL(e,r)						\
do {								\
     if (HOST_FLOAT_WORDS_BIG_ENDIAN == REAL_WORDS_BIG_ENDIAN)	\
       etoe53 ((e), (uint16_t *) (r));			\
     else							\
       {							\
	 uint16_t w[4];					\
	 etoe53 ((e), w);						\
         memcpy (((int16_t *) r), &w[3], sizeof (int16_t));		\
         memcpy (((int16_t *) r) + 1, &w[2], sizeof (int16_t));	\
         memcpy (((int16_t *) r) + 2, &w[1], sizeof (int16_t));	\
         memcpy (((int16_t *) r) + 3, &w[0], sizeof (int16_t));	\
       }							\
   } while (0)



/* Number of 16 bit words in internal format */
#define NI (NE+3)

/* Array offset to exponent */
#define E 1

/* Array offset to high guard word */
#define M 2

/* Number of bits of precision */
#define NBITS ((NI-4)*16)

/* Maximum number of decimal digits in ASCII conversion
 * = NBITS*log10(2)
 */
#define NDEC (NBITS*8/27)

/* The exponent of 1.0 */
#define EXONE (0x3fff)

extern int extra_warnings;
extern uint16_t ezero[], ehalf[], eone[], etwo[];
extern uint16_t elog2[], esqrt2[];

static void endian	(uint16_t *, long *,
			       enum machine_mode);
static void eclear	(uint16_t *);
static void emov	(uint16_t *, uint16_t *);
static void eneg	(uint16_t *);
static int eisneg	(uint16_t *);
static int eisinf	(uint16_t *);
static int eisnan	(uint16_t *);
static void einfin	(uint16_t *);
static void enan	(uint16_t *, int);
static void emovi	(uint16_t *, uint16_t *);
static void emovo	(uint16_t *, uint16_t *);
static void ecleaz	(uint16_t *);
static void ecleazs	(uint16_t *);
static void emovz	(uint16_t *, uint16_t *);
static void einan	(uint16_t *);
static int eiisnan	(uint16_t *);
static int eiisneg	(uint16_t *);
static int ecmpm	(uint16_t *, uint16_t *);
static void eshdn1	(uint16_t *);
static void eshup1	(uint16_t *);
static void eshdn8	(uint16_t *);
static void eshup8	(uint16_t *);
static void eshup6	(uint16_t *);
static void eshdn6	(uint16_t *);
static void eaddm	(uint16_t *, uint16_t *);
static void esubm	(uint16_t *, uint16_t *);
static void m16m	(unsigned int, unsigned short *,
			       unsigned short *);
static int edivm	(unsigned short *, unsigned short *);
static int emulm	(unsigned short *, unsigned short *);
static void emdnorm	(uint16_t *, int, int, int32_t, int);
static void esub	(uint16_t *, uint16_t *,
			       uint16_t *);
static void eadd	(uint16_t *, uint16_t *,
			       uint16_t *);
static void eadd1	(uint16_t *, uint16_t *,
			       uint16_t *);
static void ediv	(uint16_t *, uint16_t *,
			       uint16_t *);
static void emul	(uint16_t *, uint16_t *,
			       uint16_t *);
static void e53toe	(uint16_t *, uint16_t *);
static void e24toe	(uint16_t *, uint16_t *);
static void etoe53	(uint16_t *, uint16_t *);
static void toe53	(uint16_t *, uint16_t *);
static void etoe24	(uint16_t *, uint16_t *);
static void toe24	(uint16_t *, uint16_t *);
static int ecmp		(uint16_t *, uint16_t *);
static void ltoe	(HOST_WIDE_INT *, uint16_t *);
static void ultoe	(HOST_WIDE_UINT *, uint16_t *);
static void eifrac	(uint16_t *, HOST_WIDE_INT *,
			       uint16_t *);
static void euifrac	(uint16_t *, HOST_WIDE_UINT *,
			       uint16_t *);
static int eshift	(uint16_t *, int);
static int enormlz	(uint16_t *);
static void etoasc	(uint16_t *, char *, int);
static void asctoe24	(char *, uint16_t *);
static void asctoe53	(char *, uint16_t *);
static void asctoe	(char *, uint16_t *);
static void asctoeg	(char *, uint16_t *, int);
static void efloor	(uint16_t *, uint16_t *);
static void eldexp	(uint16_t *, int, uint16_t *);
static void eiremain	(uint16_t *, uint16_t *);
static void mtherr	(char *, int);
static void make_nan	(uint16_t *, int, enum machine_mode);

/* Copy 32-bit numbers obtained from array containing 16-bit numbers,
   swapping ends if required, into output array of longs.  The
   result is normally passed to fprintf by the ASM_OUTPUT_ macros.   */

static void
endian (e, x, mode)
     uint16_t e[];
     long x[];
     enum machine_mode mode;
{
  unsigned long th, t;

  if (REAL_WORDS_BIG_ENDIAN)
    {
      switch (mode)
	{
	case DFmode:
	  /* Swap halfwords in the second word.  */
	  th = (unsigned long) e[2] & 0xffff;
	  t = (unsigned long) e[3] & 0xffff;
	  t |= th << 16;
	  x[1] = (long) t;
	  /* fall into the float case */

	case SFmode:
	  /* Swap halfwords in the first word.  */
	  th = (unsigned long) e[0] & 0xffff;
	  t = (unsigned long) e[1] & 0xffff;
	  t |= th << 16;
	  x[0] = (long) t;
	  break;

	default:
	  abort ();
	}
    }
  else
    {
      /* Pack the output array without swapping.  */

      switch (mode)
	{
	case DFmode:
	  /* Pack the second long */
	  th = (unsigned long) e[3] & 0xffff;
	  t = (unsigned long) e[2] & 0xffff;
	  t |= th << 16;
	  x[1] = (long) t;
	  /* fall into the float case */

	case SFmode:
	  /* Pack the first long */
	  th = (unsigned long) e[1] & 0xffff;
	  t = (unsigned long) e[0] & 0xffff;
	  t |= th << 16;
	  x[0] = (long) t;
	  break;

	default:
	  abort ();
	}
    }

  /* If 32 bits is an entire word for the target, but not for the host,
     then sign-extend on the host so that the number will look the same
     way on the host that it would on the target.  See for instance
     simplify_unary_operation.  The #if is needed to avoid compiler
     warnings.  */

#if HOST_BITS_PER_WIDE_INT > 32
  if (BITS_PER_WORD < HOST_BITS_PER_WIDE_INT && BITS_PER_WORD == 32)
    {
      if (x[0] & ((HOST_WIDE_INT) 1 << 31))
	x[0] |= ((HOST_WIDE_INT) (-1) << 32);

      if (x[1] & ((HOST_WIDE_INT) 1 << 31))
	x[1] |= ((HOST_WIDE_INT) (-1) << 32);
    }
#endif
}


/* This is the implementation of the REAL_ARITHMETIC macro.  */

void
earith (value, icode, r1, r2)
     REAL_VALUE_TYPE *value;
     int icode;
     REAL_VALUE_TYPE *r1;
     REAL_VALUE_TYPE *r2;
{
  uint16_t d1[NE], d2[NE], v[NE];
  enum tree_code code;

  GET_REAL (r1, d1);
  GET_REAL (r2, d2);
/*  Return NaN input back to the caller.  */
  if (eisnan (d1))
    {
      PUT_REAL (d1, value);
      return;
    }
  if (eisnan (d2))
    {
      PUT_REAL (d2, value);
      return;
    }
  code = (enum tree_code) icode;
  switch (code)
    {
    case PLUS_EXPR:
      eadd (d2, d1, v);
      break;

    case MINUS_EXPR:
      esub (d2, d1, v);		/* d1 - d2 */
      break;

    case MULT_EXPR:
      emul (d2, d1, v);
      break;

    case RDIV_EXPR:
#ifndef REAL_INFINITY
      if (ecmp (d2, ezero) == 0)
	{
	enan (v, eisneg (d1) ^ eisneg (d2));
	break;
	}
#endif
      ediv (d2, d1, v);	/* d1/d2 */
      break;

    case MIN_EXPR:		/* min (d1,d2) */
      if (ecmp (d1, d2) < 0)
	emov (d1, v);
      else
	emov (d2, v);
      break;

    case MAX_EXPR:		/* max (d1,d2) */
      if (ecmp (d1, d2) > 0)
	emov (d1, v);
      else
	emov (d2, v);
      break;
    default:
      emov (ezero, v);
      break;
    }
PUT_REAL (v, value);
}


/* Truncate REAL_VALUE_TYPE toward zero to signed HOST_WIDE_INT.
   implements REAL_VALUE_RNDZINT (x) (etrunci (x)).  */

REAL_VALUE_TYPE
etrunci (x)
     REAL_VALUE_TYPE x;
{
  uint16_t f[NE], g[NE];
  REAL_VALUE_TYPE r;
  HOST_WIDE_INT l;

  GET_REAL (&x, g);
  if (eisnan (g))
    return (x);
  eifrac (g, &l, f);
  ltoe (&l, g);
  PUT_REAL (g, &r);
  return (r);
}


/* Truncate REAL_VALUE_TYPE toward zero to HOST_WIDE_UINT;
   implements REAL_VALUE_UNSIGNED_RNDZINT (x) (etruncui (x)).  */

REAL_VALUE_TYPE
etruncui (x)
     REAL_VALUE_TYPE x;
{
  uint16_t f[NE], g[NE];
  REAL_VALUE_TYPE r;
  HOST_WIDE_UINT l;

  GET_REAL (&x, g);
  if (eisnan (g))
    return (x);
  euifrac (g, &l, f);
  ultoe (&l, g);
  PUT_REAL (g, &r);
  return (r);
}


/* This is the REAL_VALUE_ATOF function.  It converts a decimal or hexadecimal
   string to binary, rounding off as indicated by the machine_mode argument.
   Then it promotes the rounded value to REAL_VALUE_TYPE.  */

REAL_VALUE_TYPE
ereal_atof (s, t)
     char *s;
     enum machine_mode t;
{
  uint16_t tem[NE], e[NE];
  REAL_VALUE_TYPE r;

  switch (t)
    {
    case SFmode:
      asctoe24 (s, tem);
      e24toe (tem, e);
      break;

    case DFmode:
      asctoe53 (s, tem);
      e53toe (tem, e);
      break;

    default:
      asctoe (s, e);
    }
  PUT_REAL (e, &r);
  return (r);
}


/* Expansion of REAL_NEGATE.  */

REAL_VALUE_TYPE
ereal_negate (x)
     REAL_VALUE_TYPE x;
{
  uint16_t e[NE];
  REAL_VALUE_TYPE r;

  GET_REAL (&x, e);
  eneg (e);
  PUT_REAL (e, &r);
  return (r);
}


/* Round real toward zero to HOST_WIDE_INT;
   implements REAL_VALUE_FIX (x).  */

HOST_WIDE_INT
efixi (x)
     REAL_VALUE_TYPE x;
{
  uint16_t f[NE], g[NE];
  HOST_WIDE_INT l;

  GET_REAL (&x, f);
  if (eisnan (f))
    {
      warning ("conversion from NaN to int");
      return (-1);
    }
  eifrac (f, &l, g);
  return l;
}

/* Round real toward zero to HOST_WIDE_UINT
   implements  REAL_VALUE_UNSIGNED_FIX (x).
   Negative input returns zero.  */

HOST_WIDE_UINT
efixui (x)
     REAL_VALUE_TYPE x;
{
  uint16_t f[NE], g[NE];
  HOST_WIDE_UINT l;

  GET_REAL (&x, f);
  if (eisnan (f))
    {
      warning ("conversion from NaN to unsigned int");
      return (-1);
    }
  euifrac (f, &l, g);
  return l;
}


/* REAL_VALUE_FROM_INT macro.  */

void
ereal_from_int (d, i, j, mode)
     REAL_VALUE_TYPE *d;
     HOST_WIDE_INT i, j;
     enum machine_mode mode;
{
  uint16_t df[NE], dg[NE];
  HOST_WIDE_INT low, high;
  int sign;

  if (GET_MODE_CLASS (mode) != MODE_FLOAT)
    abort ();
  sign = 0;
  low = i;
  if ((high = j) < 0)
    {
      sign = 1;
      /* complement and add 1 */
      high = ~high;
      if (low)
	low = -low;
      else
	high += 1;
    }
  eldexp (eone, HOST_BITS_PER_WIDE_INT, df);
  ultoe ((HOST_WIDE_UINT *) &high, dg);
  emul (dg, df, dg);
  ultoe ((HOST_WIDE_UINT *) &low, df);
  eadd (df, dg, dg);
  if (sign)
    eneg (dg);

  /* A REAL_VALUE_TYPE may not be wide enough to hold the two HOST_WIDE_INTS.
     Avoid double-rounding errors later by rounding off now from the
     extra-wide internal format to the requested precision.  */
  switch (GET_MODE_BITSIZE (mode))
    {
    case 32:
      etoe24 (dg, df);
      e24toe (df, dg);
      break;

    case 64:
      etoe53 (dg, df);
      e53toe (df, dg);
      break;

    default:
      abort ();
  }

  PUT_REAL (dg, d);
}


/* REAL_VALUE_FROM_UNSIGNED_INT macro.   */

void
ereal_from_uint (d, i, j, mode)
     REAL_VALUE_TYPE *d;
     HOST_WIDE_UINT i, j;
     enum machine_mode mode;
{
  uint16_t df[NE], dg[NE];
  HOST_WIDE_UINT low, high;

  if (GET_MODE_CLASS (mode) != MODE_FLOAT)
    abort ();
  low = i;
  high = j;
  eldexp (eone, HOST_BITS_PER_WIDE_INT, df);
  ultoe (&high, dg);
  emul (dg, df, dg);
  ultoe (&low, df);
  eadd (df, dg, dg);

  /* A REAL_VALUE_TYPE may not be wide enough to hold the two HOST_WIDE_INTS.
     Avoid double-rounding errors later by rounding off now from the
     extra-wide internal format to the requested precision.  */
  switch (GET_MODE_BITSIZE (mode))
    {
    case 32:
      etoe24 (dg, df);
      e24toe (df, dg);
      break;

    case 64:
      etoe53 (dg, df);
      e53toe (df, dg);
      break;

    default:
      abort ();
  }

  PUT_REAL (dg, d);
}


/* REAL_VALUE_TO_INT macro.  */

void
ereal_to_int (low, high, rr)
     HOST_WIDE_INT *low, *high;
     REAL_VALUE_TYPE rr;
{
  uint16_t d[NE], df[NE], dg[NE], dh[NE];
  int s;

  GET_REAL (&rr, d);
  if (eisnan (d))
    {
      warning ("conversion from NaN to int");
      *low = -1;
      *high = -1;
      return;
    }
  /* convert positive value */
  s = 0;
  if (eisneg (d))
    {
      eneg (d);
      s = 1;
    }
  eldexp (eone, HOST_BITS_PER_WIDE_INT, df);
  ediv (df, d, dg);		/* dg = d / 2^32 is the high word */
  euifrac (dg, (HOST_WIDE_UINT *) high, dh);
  emul (df, dh, dg);		/* fractional part is the low word */
  euifrac (dg, (HOST_WIDE_UINT *)low, dh);
  if (s)
    {
      /* complement and add 1 */
      *high = ~(*high);
      if (*low)
	*low = -(*low);
      else
	*high += 1;
    }
}


/* REAL_VALUE_LDEXP macro.  */

REAL_VALUE_TYPE
ereal_ldexp (x, n)
     REAL_VALUE_TYPE x;
     int n;
{
  uint16_t e[NE], y[NE];
  REAL_VALUE_TYPE r;

  GET_REAL (&x, e);
  if (eisnan (e))
    return (x);
  eldexp (e, n, y);
  PUT_REAL (y, &r);
  return (r);
}

/* These routines are conditionally compiled because functions
   of the same names may be defined in fold-const.c.  */


/* Check for infinity in a REAL_VALUE_TYPE.  */

int
target_isinf (x)
     REAL_VALUE_TYPE x;
{
  uint16_t e[NE];

  GET_REAL (&x, e);
  return (eisinf (e));
}

/* Check whether a REAL_VALUE_TYPE item is a NaN.  */

int
target_isnan (x)
     REAL_VALUE_TYPE x;
{
  uint16_t e[NE];

  GET_REAL (&x, e);
  return (eisnan (e));
}


/* Check for a negative REAL_VALUE_TYPE number.
   This just checks the sign bit, so that -0 counts as negative.  */

int
target_negative (x)
     REAL_VALUE_TYPE x;
{
  return ereal_isneg (x);
}

/* Expansion of REAL_VALUE_TRUNCATE.
   The result is in floating point, rounded to nearest or even.  */

REAL_VALUE_TYPE
real_value_truncate (mode, arg)
     enum machine_mode mode;
     REAL_VALUE_TYPE arg;
{
  uint16_t e[NE], t[NE];
  REAL_VALUE_TYPE r;

  GET_REAL (&arg, e);
  if (eisnan (e))
    return (arg);
  eclear (t);
  switch (mode)
    {
    case DFmode:
      etoe53 (e, t);
      e53toe (t, t);
      break;

    case SFmode:
      etoe24 (e, t);
      e24toe (t, t);
      break;


    case SImode:
      r = etrunci (arg);
      return (r);

    /* If an unsupported type was requested, presume that
       the machine files know something useful to do with
       the unmodified value.  */

    default:
      return (arg);
    }
  PUT_REAL (t, &r);
  return (r);
}

/* Try to change R into its exact multiplicative inverse in machine mode
   MODE.  Return nonzero function value if successful.  */

int
exact_real_inverse (mode, r)
     enum machine_mode mode;
     REAL_VALUE_TYPE *r;
{
  uint16_t e[NE], einv[NE];
  REAL_VALUE_TYPE rinv;
  int i;

  GET_REAL (r, e);

  /* Test for input in range.  Don't transform IEEE special values.  */
  if (eisinf (e) || eisnan (e) || (ecmp (e, ezero) == 0))
    return 0;

  /* Test for a power of 2: all significand bits zero except the MSB.
     We are assuming the target has binary (or hex) arithmetic.  */
  if (e[NE - 2] != 0x8000)
    return 0;

  for (i = 0; i < NE - 2; i++)
    {
      if (e[i] != 0)
	return 0;
    }

  /* Compute the inverse and truncate it to the required mode.  */
  ediv (e, eone, einv);
  PUT_REAL (einv, &rinv);
  rinv = real_value_truncate (mode, rinv);

#ifdef CHECK_FLOAT_VALUE
  /* This check is not redundant.  It may, for example, flush
     a supposedly IEEE denormal value to zero.  */
  i = 0;
  if (CHECK_FLOAT_VALUE (mode, rinv, i))
    return 0;
#endif
  GET_REAL (&rinv, einv);

  /* Check the bits again, because the truncation might have
     generated an arbitrary saturation value on overflow.  */
  if (einv[NE - 2] != 0x8000)
    return 0;

  for (i = 0; i < NE - 2; i++)
    {
      if (einv[i] != 0)
	return 0;
    }

  /* Fail if the computed inverse is out of range.  */
  if (eisinf (einv) || eisnan (einv) || (ecmp (einv, ezero) == 0))
    return 0;

  /* Output the reciprocal and return success flag.  */
  PUT_REAL (einv, r);
  return 1;
}

/* Used for debugging--print the value of R in human-readable format
   on stderr.  */

void
debug_real (r)
     REAL_VALUE_TYPE r;
{
  char dstr[30];

  REAL_VALUE_TO_DECIMAL (r, "%.20g", dstr);
  fprintf (stderr, "%s", dstr);
}


/* The following routines convert REAL_VALUE_TYPE to the various floating
   point formats that are meaningful to supported computers.

   The results are returned in 32-bit pieces, each piece stored in a `long'.
   This is so they can be printed by statements like

      fprintf (file, "%lx, %lx", L[0],  L[1]);

   that will work on both narrow- and wide-word host computers.  */

/* Convert R to a double precision value.  The output array L contains two
   32-bit pieces of the result, in the order they would appear in memory.  */

void
etardouble (r, l)
     REAL_VALUE_TYPE r;
     long l[];
{
  uint16_t e[NE];

  GET_REAL (&r, e);
  etoe53 (e, e);
  endian (e, l, DFmode);
}

/* Convert R to a single precision float value stored in the least-significant
   bits of a `long'.  */

long
etarsingle (r)
     REAL_VALUE_TYPE r;
{
  uint16_t e[NE];
  long l;

  GET_REAL (&r, e);
  etoe24 (e, e);
  endian (e, &l, SFmode);
  return ((long) l);
}

/* Convert X to a decimal ASCII string S for output to an assembly
   language file.  Note, there is no standard way to spell infinity or
   a NaN, so these values may require special treatment in the tm.h
   macros.  */

void
ereal_to_decimal (x, s)
     REAL_VALUE_TYPE x;
     char *s;
{
  uint16_t e[NE];

  GET_REAL (&x, e);
  etoasc (e, s, 20);
}

/* Compare X and Y.  Return 1 if X > Y, 0 if X == Y, -1 if X < Y,
   or -2 if either is a NaN.   */

int
ereal_cmp (x, y)
     REAL_VALUE_TYPE x, y;
{
  uint16_t ex[NE], ey[NE];

  GET_REAL (&x, ex);
  GET_REAL (&y, ey);
  return (ecmp (ex, ey));
}

/*  Return 1 if the sign bit of X is set, else return 0.  */

int
ereal_isneg (x)
     REAL_VALUE_TYPE x;
{
  uint16_t ex[NE];

  GET_REAL (&x, ex);
  return (eisneg (ex));
}

/* End of REAL_ARITHMETIC interface */

/*
  Extended precision IEEE binary floating point arithmetic routines

  Numbers are stored in C language as arrays of 16-bit unsigned
  short integers.  The arguments of the routines are pointers to
  the arrays.

  External e type data structure, similar to Intel 8087 chip
  temporary real format but possibly with a larger significand:

	NE-1 significand words	(least significant word first,
				 most significant bit is normally set)
	exponent		(value = EXONE for 1.0,
				top bit is the sign)


  Internal exploded e-type data structure of a number (a "word" is 16 bits):

  ei[0]	sign word	(0 for positive, 0xffff for negative)
  ei[1]	biased exponent	(value = EXONE for the number 1.0)
  ei[2]	high guard word	(always zero after normalization)
  ei[3]
  to ei[NI-2]	significand	(NI-4 significand words,
 				 most significant word first,
 				 most significant bit is set)
  ei[NI-1]	low guard word	(0x8000 bit is rounding place)



 		Routines for external format e-type numbers

 	asctoe (string, e)	ASCII string to extended double e type
 	asctoe64 (string, &d)	ASCII string to long double
 	asctoe53 (string, &d)	ASCII string to double
 	asctoe24 (string, &f)	ASCII string to single
 	asctoeg (string, e, prec) ASCII string to specified precision
 	e24toe (&f, e)		IEEE single precision to e type
 	e53toe (&d, e)		IEEE double precision to e type
 	e64toe (&d, e)		IEEE long double precision to e type
 	e113toe (&d, e)		128-bit long double precision to e type
#if 0
 	eabs (e)			absolute value
#endif
 	eadd (a, b, c)		c = b + a
 	eclear (e)		e = 0
 	ecmp (a, b)		Returns 1 if a > b, 0 if a == b,
 				-1 if a < b, -2 if either a or b is a NaN.
 	ediv (a, b, c)		c = b / a
 	efloor (a, b)		truncate to integer, toward -infinity
 	efrexp (a, exp, s)	extract exponent and significand
 	eifrac (e, &l, frac)    e to HOST_WIDE_INT and e type fraction
 	euifrac (e, &l, frac)   e to HOST_WIDE_UINT and e type fraction
 	einfin (e)		set e to infinity, leaving its sign alone
 	eldexp (a, n, b)	multiply by 2**n
 	emov (a, b)		b = a
 	emul (a, b, c)		c = b * a
 	eneg (e)			e = -e
#if 0
 	eround (a, b)		b = nearest integer value to a
#endif
 	esub (a, b, c)		c = b - a
#if 0
 	e24toasc (&f, str, n)	single to ASCII string, n digits after decimal
 	e53toasc (&d, str, n)	double to ASCII string, n digits after decimal
 	e64toasc (&d, str, n)	80-bit long double to ASCII string
 	e113toasc (&d, str, n)	128-bit long double to ASCII string
#endif
 	etoasc (e, str, n)	e to ASCII string, n digits after decimal
 	etoe24 (e, &f)		convert e type to IEEE single precision
 	etoe53 (e, &d)		convert e type to IEEE double precision
 	etoe64 (e, &d)		convert e type to IEEE long double precision
 	ltoe (&l, e)		HOST_WIDE_INT to e type
 	ultoe (&l, e)		HOST_WIDE_UINT to e type
	eisneg (e)              1 if sign bit of e != 0, else 0
	eisinf (e)              1 if e has maximum exponent (non-IEEE)
 				or is infinite (IEEE)
        eisnan (e)              1 if e is a NaN


 		Routines for internal format exploded e-type numbers

 	eaddm (ai, bi)		add significands, bi = bi + ai
 	ecleaz (ei)		ei = 0
 	ecleazs (ei)		set ei = 0 but leave its sign alone
 	ecmpm (ai, bi)		compare significands, return 1, 0, or -1
 	edivm (ai, bi)		divide  significands, bi = bi / ai
 	emdnorm (ai,l,s,exp)	normalize and round off
 	emovi (a, ai)		convert external a to internal ai
 	emovo (ai, a)		convert internal ai to external a
 	emovz (ai, bi)		bi = ai, low guard word of bi = 0
 	emulm (ai, bi)		multiply significands, bi = bi * ai
 	enormlz (ei)		left-justify the significand
 	eshdn1 (ai)		shift significand and guards down 1 bit
 	eshdn8 (ai)		shift down 8 bits
 	eshdn6 (ai)		shift down 16 bits
 	eshift (ai, n)		shift ai n bits up (or down if n < 0)
 	eshup1 (ai)		shift significand and guards up 1 bit
 	eshup8 (ai)		shift up 8 bits
 	eshup6 (ai)		shift up 16 bits
 	esubm (ai, bi)		subtract significands, bi = bi - ai
        eiisinf (ai)            1 if infinite
        eiisnan (ai)            1 if a NaN
 	eiisneg (ai)		1 if sign bit of ai != 0, else 0
        einan (ai)              set ai = NaN
#if 0
        eiinfin (ai)            set ai = infinity
#endif

  The result is always normalized and rounded to NI-4 word precision
  after each arithmetic operation.

  Exception flags are NOT fully supported.

  Signaling NaN's are NOT supported; they are treated the same
  as quiet NaN's.

  Define INFINITY for support of infinity; otherwise a
  saturation arithmetic is implemented.

  Define NANS for support of Not-a-Number items; otherwise the
  arithmetic will never produce a NaN output, and might be confused
  by a NaN input.
  If NaN's are supported, the output of `ecmp (a,b)' is -2 if
  either a or b is a NaN. This means asking `if (ecmp (a,b) < 0)'
  may not be legitimate. Use `if (ecmp (a,b) == -1)' for `less than'
  if in doubt.

  Denormals are always supported here where appropriate (e.g., not
  for conversion to DEC numbers).  */

/* Definitions for error codes that are passed to the common error handling
   routine mtherr.

   For Digital Equipment PDP-11 and VAX computers, certain
  IBM systems, and others that use numbers with a 56-bit
  significand, the symbol DEC should be defined.  In this
  mode, most floating point constants are given as arrays
  of octal integers to eliminate decimal to binary conversion
  errors that might be introduced by the compiler.

  For computers, such as IBM PC, that follow the IEEE
  Standard for Binary Floating Point Arithmetic (ANSI/IEEE
  Std 754-1985), the symbol IEEE should be defined.
  These numbers have 53-bit significands.  In this mode, constants
  are provided as arrays of hexadecimal 16 bit integers.
  The endian-ness of generated values is controlled by
  REAL_WORDS_BIG_ENDIAN.

  To accommodate other types of computer arithmetic, all
  constants are also provided in a normal decimal radix
  which one can hope are correctly converted to a suitable
  format by the available C language compiler.  To invoke
  this mode, the symbol UNK is defined.

  An important difference among these modes is a predefined
  set of machine arithmetic constants for each.  The numbers
  MACHEP (the machine roundoff error), MAXNUM (largest number
  represented), and several other parameters are preset by
  the configuration symbol.  Check the file const.c to
  ensure that these values are correct for your computer.

  For ANSI C compatibility, define ANSIC equal to 1.  Currently
  this affects only the atan2 function and others that use it.  */

/* Constant definitions for math error conditions.  */

#define DOMAIN		1	/* argument domain error */
#define SING		2	/* argument singularity */
#define OVERFLOW	3	/* overflow range error */
#define UNDERFLOW	4	/* underflow range error */
#define TLOSS		5	/* total loss of precision */
#define PLOSS		6	/* partial loss of precision */
#define INVALID		7	/* NaN-producing operation */

/*  e type constants used by high precision check routines */

/* LONG_DOUBLE_TYPE_SIZE is other than 128 */
uint16_t ezero[NE] =
 {0, 0000000, 0000000, 0000000, 0000000, 0000000,};
uint16_t ehalf[NE] =
 {0, 0000000, 0000000, 0000000, 0100000, 0x3ffe,};
uint16_t eone[NE] =
 {0, 0000000, 0000000, 0000000, 0100000, 0x3fff,};
uint16_t etwo[NE] =
 {0, 0000000, 0000000, 0000000, 0100000, 0040000,};
uint16_t e32[NE] =
 {0, 0000000, 0000000, 0000000, 0100000, 0040004,};
uint16_t elog2[NE] =
 {0xc9e4, 0x79ab, 0150717, 0013767, 0130562, 0x3ffe,};
uint16_t esqrt2[NE] =
 {0x597e, 0x6484, 0174736, 0171463, 0132404, 0x3fff,};
uint16_t epi[NE] =
 {0xc4c6, 0xc234, 0020550, 0155242, 0144417, 0040000,};

/* Control register for rounding precision.
   This can be set to 113 (if NE=10), 80 (if NE=6), 64, 56, 53, or 24 bits.  */

int rndprc = NBITS;
extern int rndprc;

/*  Clear out entire e-type number X.  */

static void
eclear (x)
     register uint16_t *x;
{
  register int i;

  for (i = 0; i < NE; i++)
    *x++ = 0;
}

/* Move e-type number from A to B.  */

static void
emov (a, b)
     register uint16_t *a, *b;
{
  register int i;

  for (i = 0; i < NE; i++)
    *b++ = *a++;
}



/* Negate the e-type number X.  */

static void
eneg (x)
     uint16_t x[];
{

  x[NE - 1] ^= 0x8000;		/* Toggle the sign bit */
}

/* Return 1 if sign bit of e-type number X is nonzero, else zero.  */

static int
eisneg (x)
     uint16_t x[];
{

  if (x[NE - 1] & 0x8000)
    return (1);
  else
    return (0);
}

/* Return 1 if e-type number X is infinity, else return zero.  */

static int
eisinf (x)
     uint16_t x[];
{

  if (eisnan (x))
    return (0);
  if ((x[NE - 1] & 0x7fff) == 0x7fff)
    return (1);
  else
    return (0);
}

/* Check if e-type number is not a number.  The bit pattern is one that we
   defined, so we know for sure how to detect it.  */

static int
eisnan (x)
     uint16_t x[];
{
  int i;

  /* NaN has maximum exponent */
  if ((x[NE - 1] & 0x7fff) != 0x7fff)
    return (0);
  /* ... and non-zero significand field.  */
  for (i = 0; i < NE - 1; i++)
    {
      if (*x++ != 0)
        return (1);
    }

  return (0);
}

/*  Fill e-type number X with infinity pattern (IEEE)
    or largest possible number (non-IEEE).  */

static void
einfin (x)
     register uint16_t *x;
{
  register int i;

  for (i = 0; i < NE - 1; i++)
    *x++ = 0;
  *x |= 32767;
}

/* Output an e-type NaN.
   This generates Intel's quiet NaN pattern for extended real.
   The exponent is 7fff, the leading mantissa word is c000.  */

static void
enan (x, sign)
     register uint16_t *x;
     int sign;
{
  register int i;

  for (i = 0; i < NE - 2; i++)
    *x++ = 0;
  *x++ = 0xc000;
  *x = (sign << 15) | 0x7fff;
}

/* Move in an e-type number A, converting it to exploded e-type B.  */

static void
emovi (a, b)
     uint16_t *a, *b;
{
  register uint16_t *p, *q;
  int i;

  q = b;
  p = a + (NE - 1);		/* point to last word of external number */
  /* get the sign bit */
  if (*p & 0x8000)
    *q++ = 0xffff;
  else
    *q++ = 0;
  /* get the exponent */
  *q = *p--;
  *q++ &= 0x7fff;		/* delete the sign bit */
  if ((*(q - 1) & 0x7fff) == 0x7fff)
    {
      if (eisnan (a))
	{
	  *q++ = 0;
	  for (i = 3; i < NI; i++)
	    *q++ = *p--;
	  return;
	}

      for (i = 2; i < NI; i++)
	*q++ = 0;
      return;
    }

  /* clear high guard word */
  *q++ = 0;
  /* move in the significand */
  for (i = 0; i < NE - 1; i++)
    *q++ = *p--;
  /* clear low guard word */
  *q = 0;
}

/* Move out exploded e-type number A, converting it to e type B.  */

static void
emovo (a, b)
     uint16_t *a, *b;
{
  register uint16_t *p, *q;
  uint16_t i;
  int j;

  p = a;
  q = b + (NE - 1);		/* point to output exponent */
  /* combine sign and exponent */
  i = *p++;
  if (i)
    *q-- = *p++ | 0x8000;
  else
    *q-- = *p++;
  if (*(p - 1) == 0x7fff)
    {
      if (eiisnan (a))
	{
	  enan (b, eiisneg (a));
	  return;
	}
      einfin (b);
	return;
    }
  /* skip over guard word */
  ++p;
  /* move the significand */
  for (j = 0; j < NE - 1; j++)
    *q-- = *p++;
}

/* Clear out exploded e-type number XI.  */

static void
ecleaz (xi)
     register uint16_t *xi;
{
  register int i;

  for (i = 0; i < NI; i++)
    *xi++ = 0;
}

/* Clear out exploded e-type XI, but don't touch the sign.  */

static void
ecleazs (xi)
     register uint16_t *xi;
{
  register int i;

  ++xi;
  for (i = 0; i < NI - 1; i++)
    *xi++ = 0;
}

/* Move exploded e-type number from A to B.  */

static void
emovz (a, b)
     register uint16_t *a, *b;
{
  register int i;

  for (i = 0; i < NI - 1; i++)
    *b++ = *a++;
  /* clear low guard word */
  *b = 0;
}

/* Generate exploded e-type NaN.
   The explicit pattern for this is maximum exponent and
   top two significant bits set.  */

static void
einan (x)
     uint16_t x[];
{

  ecleaz (x);
  x[E] = 0x7fff;
  x[M + 1] = 0xc000;
}

/* Return nonzero if exploded e-type X is a NaN.  */

static int
eiisnan (x)
     uint16_t x[];
{
  int i;

  if ((x[E] & 0x7fff) == 0x7fff)
    {
      for (i = M + 1; i < NI; i++)
	{
	  if (x[i] != 0)
	    return (1);
	}
    }
  return (0);
}

/* Return nonzero if sign of exploded e-type X is nonzero.  */

static int
eiisneg (x)
     uint16_t x[];
{

  return x[0] != 0;
}

/* Compare significands of numbers in internal exploded e-type format.
   Guard words are included in the comparison.

   Returns	+1 if a > b
		 0 if a == b
		-1 if a < b   */

static int
ecmpm (a, b)
     register uint16_t *a, *b;
{
  int i;

  a += M;			/* skip up to significand area */
  b += M;
  for (i = M; i < NI; i++)
    {
      if (*a++ != *b++)
	goto difrnt;
    }
  return (0);

 difrnt:
  if (*(--a) > *(--b))
    return (1);
  else
    return (-1);
}

/* Shift significand of exploded e-type X down by 1 bit.  */

static void
eshdn1 (x)
     register uint16_t *x;
{
  register uint16_t bits;
  int i;

  x += M;			/* point to significand area */

  bits = 0;
  for (i = M; i < NI; i++)
    {
      if (*x & 1)
	bits |= 1;
      *x >>= 1;
      if (bits & 2)
	*x |= 0x8000;
      bits <<= 1;
      ++x;
    }
}

/* Shift significand of exploded e-type X up by 1 bit.  */

static void
eshup1 (x)
     register uint16_t *x;
{
  register uint16_t bits;
  int i;

  x += NI - 1;
  bits = 0;

  for (i = M; i < NI; i++)
    {
      if (*x & 0x8000)
	bits |= 1;
      *x <<= 1;
      if (bits & 2)
	*x |= 1;
      bits <<= 1;
      --x;
    }
}


/* Shift significand of exploded e-type X down by 8 bits.  */

static void
eshdn8 (x)
     register uint16_t *x;
{
  register uint16_t newbyt, oldbyt;
  int i;

  x += M;
  oldbyt = 0;
  for (i = M; i < NI; i++)
    {
      newbyt = *x << 8;
      *x >>= 8;
      *x |= oldbyt;
      oldbyt = newbyt;
      ++x;
    }
}

/* Shift significand of exploded e-type X up by 8 bits.  */

static void
eshup8 (x)
     register uint16_t *x;
{
  int i;
  register uint16_t newbyt, oldbyt;

  x += NI - 1;
  oldbyt = 0;

  for (i = M; i < NI; i++)
    {
      newbyt = *x >> 8;
      *x <<= 8;
      *x |= oldbyt;
      oldbyt = newbyt;
      --x;
    }
}

/* Shift significand of exploded e-type X up by 16 bits.  */

static void
eshup6 (x)
     register uint16_t *x;
{
  int i;
  register uint16_t *p;

  p = x + M;
  x += M + 1;

  for (i = M; i < NI - 1; i++)
    *p++ = *x++;

  *p = 0;
}

/* Shift significand of exploded e-type X down by 16 bits.  */

static void
eshdn6 (x)
     register uint16_t *x;
{
  int i;
  register uint16_t *p;

  x += NI - 1;
  p = x + 1;

  for (i = M; i < NI - 1; i++)
    *(--p) = *(--x);

  *(--p) = 0;
}

/* Add significands of exploded e-type X and Y.  X + Y replaces Y.  */

static void
eaddm (x, y)
     uint16_t *x, *y;
{
  register uint32_t a;
  int i;
  unsigned int carry;

  x += NI - 1;
  y += NI - 1;
  carry = 0;
  for (i = M; i < NI; i++)
    {
      a = (uint32_t) (*x) + (uint32_t) (*y) + carry;
      if (a & 0x10000)
	carry = 1;
      else
	carry = 0;
      *y = (uint16_t) a;
      --x;
      --y;
    }
}

/* Subtract significands of exploded e-type X and Y.  Y - X replaces Y.  */

static void
esubm (x, y)
     uint16_t *x, *y;
{
  uint32_t a;
  int i;
  unsigned int carry;

  x += NI - 1;
  y += NI - 1;
  carry = 0;
  for (i = M; i < NI; i++)
    {
      a = (uint32_t) (*y) - (uint32_t) (*x) - carry;
      if (a & 0x10000)
	carry = 1;
      else
	carry = 0;
      *y = (uint16_t) a;
      --x;
      --y;
    }
}


static uint16_t equot[NI];



/* Radix 65536 versions of multiply and divide.  */

/* Multiply significand of e-type number B
   by 16-bit quantity A, return e-type result to C.  */

static void
m16m (a, b, c)
     unsigned int a;
     uint16_t b[], c[];
{
  register uint16_t *pp;
  register uint32_t carry;
  uint16_t *ps;
  uint16_t p[NI];
  uint32_t aa, m;
  int i;

  aa = a;
  pp = &p[NI-2];
  *pp++ = 0;
  *pp = 0;
  ps = &b[NI-1];

  for (i=M+1; i<NI; i++)
    {
      if (*ps == 0)
	{
	  --ps;
	  --pp;
	  *(pp-1) = 0;
	}
      else
	{
	  m = (uint32_t) aa * *ps--;
	  carry = (m & 0xffff) + *pp;
	  *pp-- = (uint16_t)carry;
	  carry = (carry >> 16) + (m >> 16) + *pp;
	  *pp = (uint16_t)carry;
	  *(pp-1) = carry >> 16;
	}
    }
  for (i=M; i<NI; i++)
    c[i] = p[i];
}

/* Divide significands of exploded e-types NUM / DEN.  Neither the
   numerator NUM nor the denominator DEN is permitted to have its high guard
   word nonzero.  */

static int
edivm (den, num)
     uint16_t den[], num[];
{
  int i;
  register uint16_t *p;
  uint32_t tnum;
  uint16_t j, tdenm, tquot;
  uint16_t tprod[NI+1];

  p = &equot[0];
  *p++ = num[0];
  *p++ = num[1];

  for (i=M; i<NI; i++)
    {
      *p++ = 0;
    }
  eshdn1 (num);
  tdenm = den[M+1];
  for (i=M; i<NI; i++)
    {
      /* Find trial quotient digit (the radix is 65536).  */
      tnum = (((uint32_t) num[M]) << 16) + num[M+1];

      /* Do not execute the divide instruction if it will overflow.  */
      if ((tdenm * (unsigned long)0xffff) < tnum)
	tquot = 0xffff;
      else
	tquot = tnum / tdenm;
      /* Multiply denominator by trial quotient digit.  */
      m16m ((unsigned int)tquot, den, tprod);
      /* The quotient digit may have been overestimated.  */
      if (ecmpm (tprod, num) > 0)
	{
	  tquot -= 1;
	  esubm (den, tprod);
	  if (ecmpm (tprod, num) > 0)
	    {
	      tquot -= 1;
	      esubm (den, tprod);
	    }
	}
      esubm (tprod, num);
      equot[i] = tquot;
      eshup6(num);
    }
  /* test for nonzero remainder after roundoff bit */
  p = &num[M];
  j = 0;
  for (i=M; i<NI; i++)
    {
      j |= *p++;
    }
  if (j)
    j = 1;

  for (i=0; i<NI; i++)
    num[i] = equot[i];

  return ((int)j);
}

/* Multiply significands of exploded e-type A and B, result in B.  */

static int
emulm (a, b)
     uint16_t a[], b[];
{
  uint16_t *p, *q;
  uint16_t pprod[NI];
  uint16_t j;
  int i;

  equot[0] = b[0];
  equot[1] = b[1];
  for (i=M; i<NI; i++)
    equot[i] = 0;

  j = 0;
  p = &a[NI-1];
  q = &equot[NI-1];
  for (i=M+1; i<NI; i++)
    {
      if (*p == 0)
	{
	  --p;
	}
      else
	{
	  m16m ((unsigned int) *p--, b, pprod);
	  eaddm(pprod, equot);
	}
      j |= *q;
      eshdn6(equot);
    }

  for (i=0; i<NI; i++)
    b[i] = equot[i];

  /* return flag for lost nonzero bits */
  return ((int)j);
}


/* Normalize and round off.

  The internal format number to be rounded is S.
  Input LOST is 0 if the value is exact.  This is the so-called sticky bit.

  Input SUBFLG indicates whether the number was obtained
  by a subtraction operation.  In that case if LOST is nonzero
  then the number is slightly smaller than indicated.

  Input EXP is the biased exponent, which may be negative.
  the exponent field of S is ignored but is replaced by
  EXP as adjusted by normalization and rounding.

  Input RCNTRL is the rounding control.  If it is nonzero, the
  returned value will be rounded to RNDPRC bits.

  For future reference:  In order for emdnorm to round off denormal
   significands at the right point, the input exponent must be
   adjusted to be the actual value it would have after conversion to
   the final floating point type.  This adjustment has been
   implemented for all type conversions (etoe53, etc.) and decimal
   conversions, but not for the arithmetic functions (eadd, etc.).
   Data types having standard 15-bit exponents are not affected by
   this, but SFmode and DFmode are affected. For example, ediv with
   rndprc = 24 will not round correctly to 24-bit precision if the
   result is denormal.   */

static int rlast = -1;
static int rw = 0;
static uint16_t rmsk = 0;
static uint16_t rmbit = 0;
static uint16_t rebit = 0;
static int re = 0;
static uint16_t rbit[NI];

static void
emdnorm (s, lost, subflg, exp, rcntrl)
     uint16_t s[];
     int lost;
     int subflg;
     int32_t exp;
     int rcntrl;
{
  int i, j;
  uint16_t r;

  /* Normalize */
  j = enormlz (s);

  /* a blank significand could mean either zero or infinity.  */
  exp -= j;
  if ((j > NBITS) && (exp < 32767))
    {
      ecleazs (s);
      return;
    }
  if (exp < 0L)
    {
      if (exp > (int32_t) (-NBITS - 1))
	{
	  j = (int) exp;
	  i = eshift (s, j);
	  if (i)
	    lost = 1;
	}
      else
	{
	  ecleazs (s);
	  return;
	}
    }
  /* Round off, unless told not to by rcntrl.  */
  if (rcntrl == 0)
    goto mdfin;
  /* Set up rounding parameters if the control register changed.  */
  if (rndprc != rlast)
    {
      ecleaz (rbit);
      switch (rndprc)
	{
	default:
	case NBITS:
	  rw = NI - 1;		/* low guard word */
	  rmsk = 0xffff;
	  rmbit = 0x8000;
	  re = rw - 1;
	  rebit = 1;
	  break;

	case 113:
	  rw = 10;
	  rmsk = 0x7fff;
	  rmbit = 0x4000;
	  rebit = 0x8000;
	  re = rw;
	  break;

	case 64:
	  rw = 7;
	  rmsk = 0xffff;
	  rmbit = 0x8000;
	  re = rw - 1;
	  rebit = 1;
	  break;

	  /* For DEC or IBM arithmetic */
	case 56:
	  rw = 6;
	  rmsk = 0xff;
	  rmbit = 0x80;
	  rebit = 0x100;
	  re = rw;
	  break;

	case 53:
	  rw = 6;
	  rmsk = 0x7ff;
	  rmbit = 0x0400;
	  rebit = 0x800;
	  re = rw;
	  break;

	  /* For C4x arithmetic */
	case 32:
	  rw = 5;
	  rmsk = 0xffff;
	  rmbit = 0x8000;
	  rebit = 1;
	  re = rw - 1;
	  break;

	case 24:
	  rw = 4;
	  rmsk = 0xff;
	  rmbit = 0x80;
	  rebit = 0x100;
	  re = rw;
	  break;
	}
      rbit[re] = rebit;
      rlast = rndprc;
    }

  /* Shift down 1 temporarily if the data structure has an implied
     most significant bit and the number is denormal.
     Intel long double denormals also lose one bit of precision.  */
  if ((exp <= 0) && (rndprc != NBITS)
      && ((rndprc != 64) || ((rndprc == 64) && ! REAL_WORDS_BIG_ENDIAN)))
    {
      lost |= s[NI - 1] & 1;
      eshdn1 (s);
    }
  /* Clear out all bits below the rounding bit,
     remembering in r if any were nonzero.  */
  r = s[rw] & rmsk;
  if (rndprc < NBITS)
    {
      i = rw + 1;
      while (i < NI)
	{
	  if (s[i])
	    r |= 1;
	  s[i] = 0;
	  ++i;
	}
    }
  s[rw] &= ~rmsk;
  if ((r & rmbit) != 0)
    {
      if (r == rmbit)
	{
	  if (lost == 0)
	    {			/* round to even */
	      if ((s[re] & rebit) == 0)
		goto mddone;
	    }
	  else
	    {
	      if (subflg != 0)
		goto mddone;
	    }
	}
      eaddm (rbit, s);
    }
 mddone:
/* Undo the temporary shift for denormal values.  */
  if ((exp <= 0) && (rndprc != NBITS)
      && ((rndprc != 64) || ((rndprc == 64) && ! REAL_WORDS_BIG_ENDIAN)))
    {
      eshup1 (s);
    }
  if (s[2] != 0)
    {				/* overflow on roundoff */
      eshdn1 (s);
      exp += 1;
    }
 mdfin:
  s[NI - 1] = 0;
  if (exp >= 32767L)
    {
      s[1] = 32767;
      for (i = 2; i < NI - 1; i++)
	s[i] = 0;
      if (extra_warnings)
	warning ("floating point overflow");
      return;
    }
  if (exp < 0)
    s[1] = 0;
  else
    s[1] = (uint16_t) exp;
}

/*  Subtract.  C = B - A, all e type numbers.  */

static int subflg = 0;

static void
esub (a, b, c)
     uint16_t *a, *b, *c;
{

  if (eisnan (a))
    {
      emov (a, c);
      return;
    }
  if (eisnan (b))
    {
      emov (b, c);
      return;
    }
/* Infinity minus infinity is a NaN.
   Test for subtracting infinities of the same sign.  */
  if (eisinf (a) && eisinf (b)
      && ((eisneg (a) ^ eisneg (b)) == 0))
    {
      mtherr ("esub", INVALID);
      enan (c, 0);
      return;
    }
  subflg = 1;
  eadd1 (a, b, c);
}

/* Add.  C = A + B, all e type.  */

static void
eadd (a, b, c)
     uint16_t *a, *b, *c;
{

/* NaN plus anything is a NaN.  */
  if (eisnan (a))
    {
      emov (a, c);
      return;
    }
  if (eisnan (b))
    {
      emov (b, c);
      return;
    }
/* Infinity minus infinity is a NaN.
   Test for adding infinities of opposite signs.  */
  if (eisinf (a) && eisinf (b)
      && ((eisneg (a) ^ eisneg (b)) != 0))
    {
      mtherr ("esub", INVALID);
      enan (c, 0);
      return;
    }
  subflg = 0;
  eadd1 (a, b, c);
}

/* Arithmetic common to both addition and subtraction.  */

static void
eadd1 (a, b, c)
     uint16_t *a, *b, *c;
{
  uint16_t ai[NI], bi[NI], ci[NI];
  int i, lost, j, k;
  int32_t lt, lta, ltb;

  if (eisinf (a))
    {
      emov (a, c);
      if (subflg)
	eneg (c);
      return;
    }
  if (eisinf (b))
    {
      emov (b, c);
      return;
    }
  emovi (a, ai);
  emovi (b, bi);
  if (subflg)
    ai[0] = ~ai[0];

  /* compare exponents */
  lta = ai[E];
  ltb = bi[E];
  lt = lta - ltb;
  if (lt > 0L)
    {				/* put the larger number in bi */
      emovz (bi, ci);
      emovz (ai, bi);
      emovz (ci, ai);
      ltb = bi[E];
      lt = -lt;
    }
  lost = 0;
  if (lt != 0L)
    {
      if (lt < (int32_t) (-NBITS - 1))
	goto done;		/* answer same as larger addend */
      k = (int) lt;
      lost = eshift (ai, k);	/* shift the smaller number down */
    }
  else
    {
      /* exponents were the same, so must compare significands */
      i = ecmpm (ai, bi);
      if (i == 0)
	{			/* the numbers are identical in magnitude */
	  /* if different signs, result is zero */
	  if (ai[0] != bi[0])
	    {
	      eclear (c);
	      return;
	    }
	  /* if same sign, result is double */
	  /* double denormalized tiny number */
	  if ((bi[E] == 0) && ((bi[3] & 0x8000) == 0))
	    {
	      eshup1 (bi);
	      goto done;
	    }
	  /* add 1 to exponent unless both are zero! */
	  for (j = 1; j < NI - 1; j++)
	    {
	      if (bi[j] != 0)
		{
		  ltb += 1;
		  if (ltb >= 0x7fff)
		    {
		      eclear (c);
		      if (ai[0] != 0)
			eneg (c);
		      einfin (c);
		      return;
		    }
		  break;
		}
	    }
	  bi[E] = (uint16_t) ltb;
	  goto done;
	}
      if (i > 0)
	{			/* put the larger number in bi */
	  emovz (bi, ci);
	  emovz (ai, bi);
	  emovz (ci, ai);
	}
    }
  if (ai[0] == bi[0])
    {
      eaddm (ai, bi);
      subflg = 0;
    }
  else
    {
      esubm (ai, bi);
      subflg = 1;
    }
  emdnorm (bi, lost, subflg, ltb, 64);

 done:
  emovo (bi, c);
}

/* Divide: C = B/A, all e type.  */

static void
ediv (a, b, c)
     uint16_t *a, *b, *c;
{
  uint16_t ai[NI], bi[NI];
  int i, sign;
  int32_t lt, lta, ltb;

/* IEEE says if result is not a NaN, the sign is "-" if and only if
   operands have opposite signs -- but flush -0 to 0 later if not IEEE.  */
  sign = eisneg(a) ^ eisneg(b);

/* Return any NaN input.  */
  if (eisnan (a))
    {
    emov (a, c);
    return;
    }
  if (eisnan (b))
    {
    emov (b, c);
    return;
    }
/* Zero over zero, or infinity over infinity, is a NaN.  */
  if (((ecmp (a, ezero) == 0) && (ecmp (b, ezero) == 0))
      || (eisinf (a) && eisinf (b)))
    {
    mtherr ("ediv", INVALID);
    enan (c, sign);
    return;
    }
/* Infinity over anything else is infinity.  */
  if (eisinf (b))
    {
      einfin (c);
      goto divsign;
    }
/* Anything else over infinity is zero.  */
  if (eisinf (a))
    {
      eclear (c);
      goto divsign;
    }
  emovi (a, ai);
  emovi (b, bi);
  lta = ai[E];
  ltb = bi[E];
  if (bi[E] == 0)
    {				/* See if numerator is zero.  */
      for (i = 1; i < NI - 1; i++)
	{
	  if (bi[i] != 0)
	    {
	      ltb -= enormlz (bi);
	      goto dnzro1;
	    }
	}
      eclear (c);
      goto divsign;
    }
 dnzro1:

  if (ai[E] == 0)
    {				/* possible divide by zero */
      for (i = 1; i < NI - 1; i++)
	{
	  if (ai[i] != 0)
	    {
	      lta -= enormlz (ai);
	      goto dnzro2;
	    }
	}
/* Divide by zero is not an invalid operation.
   It is a divide-by-zero operation!   */
      einfin (c);
      mtherr ("ediv", SING);
      goto divsign;
    }
 dnzro2:

  i = edivm (ai, bi);
  /* calculate exponent */
  lt = ltb - lta + EXONE;
  emdnorm (bi, i, 0, lt, 64);
  emovo (bi, c);

 divsign:

  if (sign
      )
     *(c+(NE-1)) |= 0x8000;
  else
     *(c+(NE-1)) &= ~0x8000;
}

/* Multiply e-types A and B, return e-type product C.   */

static void
emul (a, b, c)
     uint16_t *a, *b, *c;
{
  uint16_t ai[NI], bi[NI];
  int i, j, sign;
  int32_t lt, lta, ltb;

/* IEEE says if result is not a NaN, the sign is "-" if and only if
   operands have opposite signs -- but flush -0 to 0 later if not IEEE.  */
  sign = eisneg(a) ^ eisneg(b);

/* NaN times anything is the same NaN.  */
  if (eisnan (a))
    {
    emov (a, c);
    return;
    }
  if (eisnan (b))
    {
    emov (b, c);
    return;
    }
/* Zero times infinity is a NaN.  */
  if ((eisinf (a) && (ecmp (b, ezero) == 0))
      || (eisinf (b) && (ecmp (a, ezero) == 0)))
    {
    mtherr ("emul", INVALID);
    enan (c, sign);
    return;
    }
/* Infinity times anything else is infinity.  */
  if (eisinf (a) || eisinf (b))
    {
      einfin (c);
      goto mulsign;
    }
  emovi (a, ai);
  emovi (b, bi);
  lta = ai[E];
  ltb = bi[E];
  if (ai[E] == 0)
    {
      for (i = 1; i < NI - 1; i++)
	{
	  if (ai[i] != 0)
	    {
	      lta -= enormlz (ai);
	      goto mnzer1;
	    }
	}
      eclear (c);
      goto mulsign;
    }
 mnzer1:

  if (bi[E] == 0)
    {
      for (i = 1; i < NI - 1; i++)
	{
	  if (bi[i] != 0)
	    {
	      ltb -= enormlz (bi);
	      goto mnzer2;
	    }
	}
      eclear (c);
      goto mulsign;
    }
 mnzer2:

  /* Multiply significands */
  j = emulm (ai, bi);
  /* calculate exponent */
  lt = lta + ltb - (EXONE - 1);
  emdnorm (bi, j, 0, lt, 64);
  emovo (bi, c);

 mulsign:

  if (sign
      )
     *(c+(NE-1)) |= 0x8000;
  else
     *(c+(NE-1)) &= ~0x8000;
}

/* Convert double precision PE to e-type Y.  */

static void
e53toe (pe, y)
     uint16_t *pe, *y;
{
  register uint16_t r;
  register uint16_t *e, *p;
  uint16_t yy[NI];
  int denorm, k;

  e = pe;
  denorm = 0;			/* flag if denormalized number */
  ecleaz (yy);
  if (! REAL_WORDS_BIG_ENDIAN)
    e += 3;
  r = *e;
  yy[0] = 0;
  if (r & 0x8000)
    yy[0] = 0xffff;
  yy[M] = (r & 0x0f) | 0x10;
  r &= ~0x800f;			/* strip sign and 4 significand bits */
  if (r == 0x7ff0)
    {
      if (! REAL_WORDS_BIG_ENDIAN)
	{
	  if (((pe[3] & 0xf) != 0) || (pe[2] != 0)
	      || (pe[1] != 0) || (pe[0] != 0))
	    {
	      enan (y, yy[0] != 0);
	      return;
	    }
	}
      else
	{
	  if (((pe[0] & 0xf) != 0) || (pe[1] != 0)
	      || (pe[2] != 0) || (pe[3] != 0))
	    {
	      enan (y, yy[0] != 0);
	      return;
	    }
	}
      eclear (y);
      einfin (y);
      if (yy[0])
	eneg (y);
      return;
    }
  r >>= 4;
  /* If zero exponent, then the significand is denormalized.
     So take back the understood high significand bit.  */

  if (r == 0)
    {
      denorm = 1;
      yy[M] &= ~0x10;
    }
  r += EXONE - 01777;
  yy[E] = r;
  p = &yy[M + 1];
  if (! REAL_WORDS_BIG_ENDIAN)
    {
      *p++ = *(--e);
      *p++ = *(--e);
      *p++ = *(--e);
    }
  else
    {
      ++e;
      *p++ = *e++;
      *p++ = *e++;
      *p++ = *e++;
    }
  eshift (yy, -5);
  if (denorm)
    {
	/* If zero exponent, then normalize the significand.  */
      if ((k = enormlz (yy)) > NBITS)
	ecleazs (yy);
      else
	yy[E] -= (uint16_t) (k - 1);
    }
  emovo (yy, y);
}

/* Convert single precision float PE to e type Y.  */

static void
e24toe (pe, y)
     uint16_t *pe, *y;
{


  register uint16_t r;
  register uint16_t *e, *p;
  uint16_t yy[NI];
  int denorm, k;

  e = pe;
  denorm = 0;			/* flag if denormalized number */
  ecleaz (yy);
  if (! REAL_WORDS_BIG_ENDIAN)
    e += 1;
  r = *e;
  yy[0] = 0;
  if (r & 0x8000)
    yy[0] = 0xffff;
  yy[M] = (r & 0x7f) | 0200;
  r &= ~0x807f;			/* strip sign and 7 significand bits */
  if (r == 0x7f80)
    {
      if (REAL_WORDS_BIG_ENDIAN)
	{
	  if (((pe[0] & 0x7f) != 0) || (pe[1] != 0))
	    {
	      enan (y, yy[0] != 0);
	      return;
	    }
	}
      else
	{
	  if (((pe[1] & 0x7f) != 0) || (pe[0] != 0))
	    {
	      enan (y, yy[0] != 0);
	      return;
	    }
	}
      eclear (y);
      einfin (y);
      if (yy[0])
	eneg (y);
      return;
    }
  r >>= 7;
  /* If zero exponent, then the significand is denormalized.
     So take back the understood high significand bit.  */
  if (r == 0)
    {
      denorm = 1;
      yy[M] &= ~0200;
    }
  r += EXONE - 0177;
  yy[E] = r;
  p = &yy[M + 1];
  if (! REAL_WORDS_BIG_ENDIAN)
    *p++ = *(--e);
  else
    {
      ++e;
      *p++ = *e++;
    }
  eshift (yy, -8);
  if (denorm)
    {				/* if zero exponent, then normalize the significand */
      if ((k = enormlz (yy)) > NBITS)
	ecleazs (yy);
      else
	yy[E] -= (uint16_t) (k - 1);
    }
  emovo (yy, y);
}

/* e type to double precision.  */


/* Convert e-type X to IEEE double E.  */

static void
etoe53 (x, e)
     uint16_t *x, *e;
{
  uint16_t xi[NI];
  int32_t exp;
  int rndsav;

  if (eisnan (x))
    {
      make_nan (e, eisneg (x), DFmode);
      return;
    }
  emovi (x, xi);
  /* adjust exponent for offsets */
  exp = (int32_t) xi[E] - (EXONE - 0x3ff);
  if (eisinf (x))
    goto nonorm;
  /* round off to nearest or even */
  rndsav = rndprc;
  rndprc = 53;
  emdnorm (xi, 0, 0, exp, 64);
  rndprc = rndsav;
 nonorm:
  toe53 (xi, e);
}

/* Convert exploded e-type X, that has already been rounded to
   53-bit precision, to IEEE double Y.  */

static void
toe53 (x, y)
     uint16_t *x, *y;
{
  uint16_t i;
  uint16_t *p;

  if (eiisnan (x))
    {
      make_nan (y, eiisneg (x), DFmode);
      return;
    }
  p = &x[0];
  if (! REAL_WORDS_BIG_ENDIAN)
    y += 3;
  *y = 0;			/* output high order */
  if (*p++)
    *y = 0x8000;		/* output sign bit */

  i = *p++;
  if (i >= (unsigned int) 2047)
    {
      /* Saturate at largest number less than infinity.  */
      *y |= 0x7ff0;
      if (! REAL_WORDS_BIG_ENDIAN)
	{
	  *(--y) = 0;
	  *(--y) = 0;
	  *(--y) = 0;
	}
      else
	{
	  ++y;
	  *y++ = 0;
	  *y++ = 0;
	  *y++ = 0;
	}
      return;
    }
  if (i == 0)
    {
      eshift (x, 4);
    }
  else
    {
      i <<= 4;
      eshift (x, 5);
    }
  i |= *p++ & (uint16_t) 0x0f;	/* *p = xi[M] */
  *y |= (uint16_t) i;	/* high order output already has sign bit set */
  if (! REAL_WORDS_BIG_ENDIAN)
    {
      *(--y) = *p++;
      *(--y) = *p++;
      *(--y) = *p;
    }
  else
    {
      ++y;
      *y++ = *p++;
      *y++ = *p++;
      *y++ = *p++;
    }
}

/* Convert e-type X to IEEE float E.  DEC float is the same as IEEE float.  */

static void
etoe24 (x, e)
     uint16_t *x, *e;
{
  int32_t exp;
  uint16_t xi[NI];
  int rndsav;

  if (eisnan (x))
    {
      make_nan (e, eisneg (x), SFmode);
      return;
    }
  emovi (x, xi);
  /* adjust exponent for offsets */
  exp = (int32_t) xi[E] - (EXONE - 0177);
  if (eisinf (x))
    goto nonorm;
  /* round off to nearest or even */
  rndsav = rndprc;
  rndprc = 24;
  emdnorm (xi, 0, 0, exp, 64);
  rndprc = rndsav;
 nonorm:
  toe24 (xi, e);
}

/* Convert exploded e-type X, that has already been rounded to
   float precision, to IEEE float Y.  */

static void
toe24 (x, y)
     uint16_t *x, *y;
{
  uint16_t i;
  uint16_t *p;

  if (eiisnan (x))
    {
      make_nan (y, eiisneg (x), SFmode);
      return;
    }
  p = &x[0];
  if (! REAL_WORDS_BIG_ENDIAN)
    y += 1;
  *y = 0;			/* output high order */
  if (*p++)
    *y = 0x8000;		/* output sign bit */

  i = *p++;
/* Handle overflow cases.  */
  if (i >= 255)
    {
      *y |= (uint16_t) 0x7f80;
      if (! REAL_WORDS_BIG_ENDIAN)
	*(--y) = 0;
      else
	{
	  ++y;
	  *y = 0;
	}
      return;
    }
  if (i == 0)
    {
      eshift (x, 7);
    }
  else
    {
      i <<= 7;
      eshift (x, 8);
    }
  i |= *p++ & (uint16_t) 0x7f;	/* *p = xi[M] */
  /* High order output already has sign bit set.  */
  *y |= i;
  if (! REAL_WORDS_BIG_ENDIAN)
    *(--y) = *p;
  else
    {
      ++y;
      *y = *p;
    }
}

/* Compare two e type numbers.
   Return +1 if a > b
           0 if a == b
          -1 if a < b
          -2 if either a or b is a NaN.  */

static int
ecmp (a, b)
     uint16_t *a, *b;
{
  uint16_t ai[NI], bi[NI];
  register uint16_t *p, *q;
  register int i;
  int msign;

  if (eisnan (a)  || eisnan (b))
      return (-2);
  emovi (a, ai);
  p = ai;
  emovi (b, bi);
  q = bi;

  if (*p != *q)
    {				/* the signs are different */
      /* -0 equals + 0 */
      for (i = 1; i < NI - 1; i++)
	{
	  if (ai[i] != 0)
	    goto nzro;
	  if (bi[i] != 0)
	    goto nzro;
	}
      return (0);
    nzro:
      if (*p == 0)
	return (1);
      else
	return (-1);
    }
  /* both are the same sign */
  if (*p == 0)
    msign = 1;
  else
    msign = -1;
  i = NI - 1;
  do
    {
      if (*p++ != *q++)
	{
	  goto diff;
	}
    }
  while (--i > 0);

  return (0);			/* equality */

 diff:

  if (*(--p) > *(--q))
    return (msign);		/* p is bigger */
  else
    return (-msign);		/* p is littler */
}


/* Convert HOST_WIDE_INT LP to e type Y.  */

static void
ltoe (lp, y)
     HOST_WIDE_INT *lp;
     uint16_t *y;
{
  uint16_t yi[NI];
  HOST_WIDE_UINT ll;
  int k;

  ecleaz (yi);
  if (*lp < 0)
    {
      /* make it positive */
      ll = (HOST_WIDE_UINT) (-(*lp));
      yi[0] = 0xffff;		/* put correct sign in the e type number */
    }
  else
    {
      ll = (HOST_WIDE_UINT) (*lp);
    }
  /* move the long integer to yi significand area */
#if HOST_BITS_PER_WIDE_INT == 64
  yi[M] = (uint16_t) (ll >> 48);
  yi[M + 1] = (uint16_t) (ll >> 32);
  yi[M + 2] = (uint16_t) (ll >> 16);
  yi[M + 3] = (uint16_t) ll;
  yi[E] = EXONE + 47;		/* exponent if normalize shift count were 0 */
#else
  yi[M] = (uint16_t) (ll >> 16);
  yi[M + 1] = (uint16_t) ll;
  yi[E] = EXONE + 15;		/* exponent if normalize shift count were 0 */
#endif

  if ((k = enormlz (yi)) > NBITS)/* normalize the significand */
    ecleaz (yi);		/* it was zero */
  else
    yi[E] -= (uint16_t) k;/* subtract shift count from exponent */
  emovo (yi, y);		/* output the answer */
}

/* Convert HOST_WIDE_UINT LP to e type Y.  */

static void
ultoe (lp, y)
     HOST_WIDE_UINT *lp;
     uint16_t *y;
{
  uint16_t yi[NI];
  HOST_WIDE_UINT ll;
  int k;

  ecleaz (yi);
  ll = *lp;

  /* move the long integer to ayi significand area */
#if HOST_BITS_PER_WIDE_INT == 64
  yi[M] = (uint16_t) (ll >> 48);
  yi[M + 1] = (uint16_t) (ll >> 32);
  yi[M + 2] = (uint16_t) (ll >> 16);
  yi[M + 3] = (uint16_t) ll;
  yi[E] = EXONE + 47;		/* exponent if normalize shift count were 0 */
#else
  yi[M] = (uint16_t) (ll >> 16);
  yi[M + 1] = (uint16_t) ll;
  yi[E] = EXONE + 15;		/* exponent if normalize shift count were 0 */
#endif

  if ((k = enormlz (yi)) > NBITS)/* normalize the significand */
    ecleaz (yi);		/* it was zero */
  else
    yi[E] -= (uint16_t) k;  /* subtract shift count from exponent */
  emovo (yi, y);		/* output the answer */
}


/* Find signed HOST_WIDE_INT integer I and floating point fractional
   part FRAC of e-type (packed internal format) floating point input X.
   The integer output I has the sign of the input.
   The output e-type fraction FRAC is the positive fractional
   part of abs (X).  */

static void
eifrac (x, i, frac)
     uint16_t *x;
     HOST_WIDE_INT *i;
     uint16_t *frac;
{
  uint16_t xi[NI];
  int j, k;
  HOST_WIDE_UINT ll;

  emovi (x, xi);
  k = (int) xi[E] - (EXONE - 1);
  if (k <= 0)
    {
      /* if exponent <= 0, integer = 0 and real output is fraction */
      *i = 0L;
      emovo (xi, frac);
      return;
    }
  if (k > (HOST_BITS_PER_WIDE_INT - 1))
    {
      /* long integer overflow: output large integer
	 and correct fraction  */
      if (xi[0])
	*i = ((HOST_WIDE_UINT) 1) << (HOST_BITS_PER_WIDE_INT - 1);
      else
	{
	  /* Return the largest positive integer.  */
	  *i = (((HOST_WIDE_UINT) 1) << (HOST_BITS_PER_WIDE_INT - 1)) - 1;
	}
      eshift (xi, k);
      if (extra_warnings)
	warning ("overflow on truncation to integer");
    }
  else if (k > 16)
    {
      /* Shift more than 16 bits: first shift up k-16 mod 16,
	 then shift up by 16's.  */
      j = k - ((k >> 4) << 4);
      eshift (xi, j);
      ll = xi[M];
      k -= j;
      do
	{
	  eshup6 (xi);
	  ll = (ll << 16) | xi[M];
	}
      while ((k -= 16) > 0);
      *i = ll;
      if (xi[0])
	*i = -(*i);
    }
  else
      {
        /* shift not more than 16 bits */
          eshift (xi, k);
        *i = (HOST_WIDE_INT) xi[M] & 0xffff;
        if (xi[0])
	  *i = -(*i);
      }
  xi[0] = 0;
  xi[E] = EXONE - 1;
  xi[M] = 0;
  if ((k = enormlz (xi)) > NBITS)
    ecleaz (xi);
  else
    xi[E] -= (uint16_t) k;

  emovo (xi, frac);
}


/* Find HOST_WIDE_UINT integer I and floating point fractional part
   FRAC of e-type X.  A negative input yields integer output = 0 but
   correct fraction.  */

static void
euifrac (x, i, frac)
     uint16_t *x;
     HOST_WIDE_UINT *i;
     uint16_t *frac;
{
  HOST_WIDE_UINT ll;
  uint16_t xi[NI];
  int j, k;

  emovi (x, xi);
  k = (int) xi[E] - (EXONE - 1);
  if (k <= 0)
    {
      /* if exponent <= 0, integer = 0 and argument is fraction */
      *i = 0L;
      emovo (xi, frac);
      return;
    }
  if (k > HOST_BITS_PER_WIDE_INT)
    {
      /* Long integer overflow: output large integer
	 and correct fraction.
	 Note, the BSD microvax compiler says that ~(0UL)
	 is a syntax error.  */
      *i = ~(0L);
      eshift (xi, k);
      if (extra_warnings)
	warning ("overflow on truncation to unsigned integer");
    }
  else if (k > 16)
    {
      /* Shift more than 16 bits: first shift up k-16 mod 16,
	 then shift up by 16's.  */
      j = k - ((k >> 4) << 4);
      eshift (xi, j);
      ll = xi[M];
      k -= j;
      do
	{
	  eshup6 (xi);
	  ll = (ll << 16) | xi[M];
	}
      while ((k -= 16) > 0);
      *i = ll;
    }
  else
    {
      /* shift not more than 16 bits */
      eshift (xi, k);
      *i = (HOST_WIDE_INT) xi[M] & 0xffff;
    }

  if (xi[0])  /* A negative value yields unsigned integer 0.  */
    *i = 0L;

  xi[0] = 0;
  xi[E] = EXONE - 1;
  xi[M] = 0;
  if ((k = enormlz (xi)) > NBITS)
    ecleaz (xi);
  else
    xi[E] -= (uint16_t) k;

  emovo (xi, frac);
}

/* Shift the significand of exploded e-type X up or down by SC bits.  */

static int
eshift (x, sc)
     uint16_t *x;
     int sc;
{
  uint16_t lost;
  uint16_t *p;

  if (sc == 0)
    return (0);

  lost = 0;
  p = x + NI - 1;

  if (sc < 0)
    {
      sc = -sc;
      while (sc >= 16)
	{
	  lost |= *p;		/* remember lost bits */
	  eshdn6 (x);
	  sc -= 16;
	}

      while (sc >= 8)
	{
	  lost |= *p & 0xff;
	  eshdn8 (x);
	  sc -= 8;
	}

      while (sc > 0)
	{
	  lost |= *p & 1;
	  eshdn1 (x);
	  sc -= 1;
	}
    }
  else
    {
      while (sc >= 16)
	{
	  eshup6 (x);
	  sc -= 16;
	}

      while (sc >= 8)
	{
	  eshup8 (x);
	  sc -= 8;
	}

      while (sc > 0)
	{
	  eshup1 (x);
	  sc -= 1;
	}
    }
  if (lost)
    lost = 1;
  return ((int) lost);
}

/* Shift normalize the significand area of exploded e-type X.
   Return the shift count (up = positive).  */

static int
enormlz (x)
     uint16_t x[];
{
  register uint16_t *p;
  int sc;

  sc = 0;
  p = &x[M];
  if (*p != 0)
    goto normdn;
  ++p;
  if (*p & 0x8000)
    return (0);			/* already normalized */
  while (*p == 0)
    {
      eshup6 (x);
      sc += 16;

      /* With guard word, there are NBITS+16 bits available.
       Return true if all are zero.  */
      if (sc > NBITS)
	return (sc);
    }
  /* see if high byte is zero */
  while ((*p & 0xff00) == 0)
    {
      eshup8 (x);
      sc += 8;
    }
  /* now shift 1 bit at a time */
  while ((*p & 0x8000) == 0)
    {
      eshup1 (x);
      sc += 1;
      if (sc > NBITS)
	{
	  mtherr ("enormlz", UNDERFLOW);
	  return (sc);
	}
    }
  return (sc);

  /* Normalize by shifting down out of the high guard word
     of the significand */
 normdn:

  if (*p & 0xff00)
    {
      eshdn8 (x);
      sc -= 8;
    }
  while (*p != 0)
    {
      eshdn1 (x);
      sc -= 1;

      if (sc < -NBITS)
	{
	  mtherr ("enormlz", OVERFLOW);
	  return (sc);
	}
    }
  return (sc);
}

/* Powers of ten used in decimal <-> binary conversions.  */

#define NTEN 12
#define MAXP 4096

/* LONG_DOUBLE_TYPE_SIZE is other than 128 */
static uint16_t etens[NTEN + 1][NE] =
{
  {0xc94c, 0x979a, 0x8a20, 0x5202, 0xc460, 0x7525,},	/* 10**4096 */
  {0xa74d, 0x5de4, 0xc53d, 0x3b5d, 0x9e8b, 0x5a92,},	/* 10**2048 */
  {0x650d, 0x0c17, 0x8175, 0x7586, 0xc976, 0x4d48,},
  {0xcc65, 0x91c6, 0xa60e, 0xa0ae, 0xe319, 0x46a3,},
  {0xddbc, 0xde8d, 0x9df9, 0xebfb, 0xaa7e, 0x4351,},
  {0xc66f, 0x8cdf, 0x80e9, 0x47c9, 0x93ba, 0x41a8,},
  {0x3cbf, 0xa6d5, 0xffcf, 0x1f49, 0xc278, 0x40d3,},
  {0xf020, 0xb59d, 0x2b70, 0xada8, 0x9dc5, 0x4069,},
  {0x0000, 0x0000, 0x0400, 0xc9bf, 0x8e1b, 0x4034,},
  {0x0000, 0x0000, 0x0000, 0x2000, 0xbebc, 0x4019,},
  {0x0000, 0x0000, 0x0000, 0x0000, 0x9c40, 0x400c,},
  {0x0000, 0x0000, 0x0000, 0x0000, 0xc800, 0x4005,},
  {0x0000, 0x0000, 0x0000, 0x0000, 0xa000, 0x4002,},	/* 10**1 */
};

static uint16_t emtens[NTEN + 1][NE] =
{
  {0x2de4, 0x9fde, 0xd2ce, 0x04c8, 0xa6dd, 0x0ad8,},	/* 10**-4096 */
  {0x4925, 0x2de4, 0x3436, 0x534f, 0xceae, 0x256b,},	/* 10**-2048 */
  {0x87a6, 0xc0bd, 0xda57, 0x82a5, 0xa2a6, 0x32b5,},
  {0x7133, 0xd21c, 0xdb23, 0xee32, 0x9049, 0x395a,},
  {0xfa91, 0x1939, 0x637a, 0x4325, 0xc031, 0x3cac,},
  {0xac7d, 0xe4a0, 0x64bc, 0x467c, 0xddd0, 0x3e55,},
  {0x3f24, 0xe9a5, 0xa539, 0xea27, 0xa87f, 0x3f2a,},
  {0x67de, 0x94ba, 0x4539, 0x1ead, 0xcfb1, 0x3f94,},
  {0x4c2f, 0xe15b, 0xc44d, 0x94be, 0xe695, 0x3fc9,},
  {0xfdc2, 0xcefc, 0x8461, 0x7711, 0xabcc, 0x3fe4,},
  {0xd3c3, 0x652b, 0xe219, 0x1758, 0xd1b7, 0x3ff1,},
  {0x3d71, 0xd70a, 0x70a3, 0x0a3d, 0xa3d7, 0x3ff8,},
  {0xcccd, 0xcccc, 0xcccc, 0xcccc, 0xcccc, 0x3ffb,},	/* 10**-1 */
};


/* Convert e-type X to ASCII string STRING with NDIGS digits after
   the decimal point.  */

static char wstring[80];	/* working storage for ASCII output */

static void
etoasc (x, string, ndigs)
     uint16_t x[];
     char *string;
     int ndigs;
{
  int16_t digit;
  uint16_t y[NI], t[NI], u[NI], w[NI];
  uint16_t *p, *r, *ten;
  uint16_t sign;
  int i, j, k, expon, rndsav;
  char *s, *ss;
  uint16_t m;


  rndsav = rndprc;
  ss = string;
  s = wstring;
  *ss = '\0';
  *s = '\0';
  if (eisnan (x))
    {
      sprintf (wstring, " NaN ");
      goto bxit;
    }
  rndprc = NBITS;		/* set to full precision */
  emov (x, y);			/* retain external format */
  if (y[NE - 1] & 0x8000)
    {
      sign = 0xffff;
      y[NE - 1] &= 0x7fff;
    }
  else
    {
      sign = 0;
    }
  expon = 0;
  ten = &etens[NTEN][0];
  emov (eone, t);
  /* Test for zero exponent */
  if (y[NE - 1] == 0)
    {
      for (k = 0; k < NE - 1; k++)
	{
	  if (y[k] != 0)
	    goto tnzro;		/* denormalized number */
	}
      goto isone;		/* valid all zeros */
    }
 tnzro:

  /* Test for infinity.  */
  if (y[NE - 1] == 0x7fff)
    {
      if (sign)
	sprintf (wstring, " -Infinity ");
      else
	sprintf (wstring, " Infinity ");
      goto bxit;
    }

  /* Test for exponent nonzero but significand denormalized.
   * This is an error condition.
   */
  if ((y[NE - 1] != 0) && ((y[NE - 2] & 0x8000) == 0))
    {
      mtherr ("etoasc", DOMAIN);
      sprintf (wstring, "NaN");
      goto bxit;
    }

  /* Compare to 1.0 */
  i = ecmp (eone, y);
  if (i == 0)
    goto isone;

  if (i == -2)
    abort ();

  if (i < 0)
    {				/* Number is greater than 1 */
      /* Convert significand to an integer and strip trailing decimal zeros.  */
      emov (y, u);
      u[NE - 1] = EXONE + NBITS - 1;

      p = &etens[NTEN - 4][0];
      m = 16;
      do
	{
	  ediv (p, u, t);
	  efloor (t, w);
	  for (j = 0; j < NE - 1; j++)
	    {
	      if (t[j] != w[j])
		goto noint;
	    }
	  emov (t, u);
	  expon += (int) m;
	noint:
	  p += NE;
	  m >>= 1;
	}
      while (m != 0);

      /* Rescale from integer significand */
      u[NE - 1] += y[NE - 1] - (unsigned int) (EXONE + NBITS - 1);
      emov (u, y);
      /* Find power of 10 */
      emov (eone, t);
      m = MAXP;
      p = &etens[0][0];
      /* An unordered compare result shouldn't happen here.  */
      while (ecmp (ten, u) <= 0)
	{
	  if (ecmp (p, u) <= 0)
	    {
	      ediv (p, u, u);
	      emul (p, t, t);
	      expon += (int) m;
	    }
	  m >>= 1;
	  if (m == 0)
	    break;
	  p += NE;
	}
    }
  else
    {				/* Number is less than 1.0 */
      /* Pad significand with trailing decimal zeros.  */
      if (y[NE - 1] == 0)
	{
	  while ((y[NE - 2] & 0x8000) == 0)
	    {
	      emul (ten, y, y);
	      expon -= 1;
	    }
	}
      else
	{
	  emovi (y, w);
	  for (i = 0; i < NDEC + 1; i++)
	    {
	      if ((w[NI - 1] & 0x7) != 0)
		break;
	      /* multiply by 10 */
	      emovz (w, u);
	      eshdn1 (u);
	      eshdn1 (u);
	      eaddm (w, u);
	      u[1] += 3;
	      while (u[2] != 0)
		{
		  eshdn1 (u);
		  u[1] += 1;
		}
	      if (u[NI - 1] != 0)
		break;
	      if (eone[NE - 1] <= u[1])
		break;
	      emovz (u, w);
	      expon -= 1;
	    }
	  emovo (w, y);
	}
      k = -MAXP;
      p = &emtens[0][0];
      r = &etens[0][0];
      emov (y, w);
      emov (eone, t);
      while (ecmp (eone, w) > 0)
	{
	  if (ecmp (p, w) >= 0)
	    {
	      emul (r, w, w);
	      emul (r, t, t);
	      expon += k;
	    }
	  k /= 2;
	  if (k == 0)
	    break;
	  p += NE;
	  r += NE;
	}
      ediv (t, eone, t);
    }
 isone:
  /* Find the first (leading) digit.  */
  emovi (t, w);
  emovz (w, t);
  emovi (y, w);
  emovz (w, y);
  eiremain (t, y);
  digit = equot[NI - 1];
  while ((digit == 0) && (ecmp (y, ezero) != 0))
    {
      eshup1 (y);
      emovz (y, u);
      eshup1 (u);
      eshup1 (u);
      eaddm (u, y);
      eiremain (t, y);
      digit = equot[NI - 1];
      expon -= 1;
    }
  s = wstring;
  if (sign)
    *s++ = '-';
  else
    *s++ = ' ';
  /* Examine number of digits requested by caller.  */
  if (ndigs < 0)
    ndigs = 0;
  if (ndigs > NDEC)
    ndigs = NDEC;
  if (digit == 10)
    {
      *s++ = '1';
      *s++ = '.';
      if (ndigs > 0)
	{
	  *s++ = '0';
	  ndigs -= 1;
	}
      expon += 1;
    }
  else
    {
      *s++ = (char)digit + '0';
      *s++ = '.';
    }
  /* Generate digits after the decimal point.  */
  for (k = 0; k <= ndigs; k++)
    {
      /* multiply current number by 10, without normalizing */
      eshup1 (y);
      emovz (y, u);
      eshup1 (u);
      eshup1 (u);
      eaddm (u, y);
      eiremain (t, y);
      *s++ = (char) equot[NI - 1] + '0';
    }
  digit = equot[NI - 1];
  --s;
  ss = s;
  /* round off the ASCII string */
  if (digit > 4)
    {
      /* Test for critical rounding case in ASCII output.  */
      if (digit == 5)
	{
	  emovo (y, t);
	  if (ecmp (t, ezero) != 0)
	    goto roun;		/* round to nearest */
	  if ((*(s - 1) & 1) == 0)
	    goto doexp;		/* round to even */
	}
      /* Round up and propagate carry-outs */
    roun:
      --s;
      k = *s & 0x7f;
      /* Carry out to most significant digit? */
      if (k == '.')
	{
	  --s;
	  k = *s;
	  k += 1;
	  *s = (char) k;
	  /* Most significant digit carries to 10? */
	  if (k > '9')
	    {
	      expon += 1;
	      *s = '1';
	    }
	  goto doexp;
	}
      /* Round up and carry out from less significant digits */
      k += 1;
      *s = (char) k;
      if (k > '9')
	{
	  *s = '0';
	  goto roun;
	}
    }
 doexp:
  /*
     if (expon >= 0)
     sprintf (ss, "e+%d", expon);
     else
     sprintf (ss, "e%d", expon);
     */
  sprintf (ss, "e%d", expon);
 bxit:
  rndprc = rndsav;
  /* copy out the working string */
  s = string;
  ss = wstring;
  while (*ss == ' ')		/* strip possible leading space */
    ++ss;
  while ((*s++ = *ss++) != '\0')
    ;
}


/* Convert ASCII string to floating point.

   Numeric input is a free format decimal number of any length, with
   or without decimal point.  Entering E after the number followed by an
   integer number causes the second number to be interpreted as a power of
   10 to be multiplied by the first number (i.e., "scientific" notation).  */

/* Convert ASCII string S to single precision float value Y.  */

static void
asctoe24 (s, y)
     char *s;
     uint16_t *y;
{
  asctoeg (s, y, 24);
}


/* Convert ASCII string S to double precision value Y.  */

static void
asctoe53 (s, y)
     char *s;
     uint16_t *y;
{
  asctoeg (s, y, 53);
}

/* Convert ASCII string S to e type Y.  */

static void
asctoe (s, y)
     char *s;
     uint16_t *y;
{
  asctoeg (s, y, NBITS);
}

/* Convert ASCII string SS to e type Y, with a specified rounding precision
   of OPREC bits.  BASE is 16 for C9X hexadecimal floating constants.  */

static void
asctoeg (ss, y, oprec)
     char *ss;
     uint16_t *y;
     int oprec;
{
  uint16_t yy[NI], xt[NI], tt[NI];
  int esign, decflg, sgnflg, nexp, exp, prec, lost;
  int k, trail, c, rndsav;
  int32_t lexp;
  uint16_t nsign, *p;
  char *sp, *s, *lstr;
  int base = 10;

  /* Copy the input string.  */
  lstr = (char *) alloca (strlen (ss) + 1);

  s = ss;
  while (*s == ' ')		/* skip leading spaces */
    ++s;

  sp = lstr;
  while ((*sp++ = *s++) != '\0')
    ;
  s = lstr;

  if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
    {
      base = 16;
      s += 2;
    }

  rndsav = rndprc;
  rndprc = NBITS;		/* Set to full precision */
  lost = 0;
  nsign = 0;
  decflg = 0;
  sgnflg = 0;
  nexp = 0;
  exp = 0;
  prec = 0;
  ecleaz (yy);
  trail = 0;

 nxtcom:
  if (*s >= '0' && *s <= '9')
    k = *s - '0';
  else if (*s >= 'a')
    k = 10 + *s - 'a';
  else
    k = 10 + *s - 'A';
  if ((k >= 0) && (k < base))
    {
      /* Ignore leading zeros */
      if ((prec == 0) && (decflg == 0) && (k == 0))
	goto donchr;
      /* Identify and strip trailing zeros after the decimal point.  */
      if ((trail == 0) && (decflg != 0))
	{
	  sp = s;
	  while ((*sp >= '0' && *sp <= '9')
		 || (base == 16 && ((*sp >= 'a' && *sp <= 'f')
				    || (*sp >= 'A' && *sp <= 'F'))))
	    ++sp;
	  /* Check for syntax error */
	  c = *sp & 0x7f;
	  if ((base != 10 || ((c != 'e') && (c != 'E')))
	      && (base != 16 || ((c != 'p') && (c != 'P')))
	      && (c != '\0')
	      && (c != '\n') && (c != '\r') && (c != ' ')
	      && (c != ','))
	    goto error;
	  --sp;
	  while (*sp == '0')
	    *sp-- = 'z';
	  trail = 1;
	  if (*s == 'z')
	    goto donchr;
	}

      /* If enough digits were given to more than fill up the yy register,
	 continuing until overflow into the high guard word yy[2]
	 guarantees that there will be a roundoff bit at the top
	 of the low guard word after normalization.  */

      if (yy[2] == 0)
	{
	  if (base == 16)
	    {
	      if (decflg)
		nexp += 4;	/* count digits after decimal point */

	      eshup1 (yy);	/* multiply current number by 16 */
	      eshup1 (yy);
	      eshup1 (yy);
	      eshup1 (yy);
	    }
	  else
	    {
	      if (decflg)
		nexp += 1;	/* count digits after decimal point */

	      eshup1 (yy);	/* multiply current number by 10 */
	      emovz (yy, xt);
	      eshup1 (xt);
	      eshup1 (xt);
	      eaddm (xt, yy);
	    }
	  /* Insert the current digit.  */
	  ecleaz (xt);
	  xt[NI - 2] = (uint16_t) k;
	  eaddm (xt, yy);
	}
      else
	{
	  /* Mark any lost non-zero digit.  */
	  lost |= k;
	  /* Count lost digits before the decimal point.  */
	  if (decflg == 0)
	    {
	      if (base == 10)
		nexp -= 1;
	      else
		nexp -= 4;
	    }
	}
      prec += 1;
      goto donchr;
    }

  switch (*s)
    {
    case 'z':
      break;
    case 'E':
    case 'e':
    case 'P':
    case 'p':
      goto expnt;
    case '.':			/* decimal point */
      if (decflg)
	goto error;
      ++decflg;
      break;
    case '-':
      nsign = 0xffff;
      if (sgnflg)
	goto error;
      ++sgnflg;
      break;
    case '+':
      if (sgnflg)
	goto error;
      ++sgnflg;
      break;
    case ',':
    case ' ':
    case '\0':
    case '\n':
    case '\r':
      goto daldone;
    case 'i':
    case 'I':
      goto infinite;
    default:
    error:
      einan (yy);
      goto aexit;
    }
 donchr:
  ++s;
  goto nxtcom;

  /* Exponent interpretation */
 expnt:
  /* 0.0eXXX is zero, regardless of XXX.  Check for the 0.0. */
  for (k = 0; k < NI; k++)
    {
      if (yy[k] != 0)
	goto read_expnt;
    }
  goto aexit;

read_expnt:
  esign = 1;
  exp = 0;
  ++s;
  /* check for + or - */
  if (*s == '-')
    {
      esign = -1;
      ++s;
    }
  if (*s == '+')
    ++s;
  while ((*s >= '0') && (*s <= '9'))
    {
      exp *= 10;
      exp += *s++ - '0';
      if (exp > 999999)
 	break;
    }
  if (esign < 0)
    exp = -exp;
  if ((exp > MAXDECEXP) && (base == 10))
    {
 infinite:
      ecleaz (yy);
      yy[E] = 0x7fff;		/* infinity */
      goto aexit;
    }
  if ((exp < MINDECEXP) && (base == 10))
    {
 zero:
      ecleaz (yy);
      goto aexit;
    }

 daldone:
  if (base == 16)
    {
      /* Base 16 hexadecimal floating constant.  */
      if ((k = enormlz (yy)) > NBITS)
	{
	  ecleaz (yy);
	  goto aexit;
	}
      /* Adjust the exponent.  NEXP is the number of hex digits,
         EXP is a power of 2.  */
      lexp = (EXONE - 1 + NBITS) - k + yy[E] + exp - nexp;
      if (lexp > 0x7fff)
	goto infinite;
      if (lexp < 0)
	goto zero;
      yy[E] = lexp;
      goto expdon;
    }

  nexp = exp - nexp;
  /* Pad trailing zeros to minimize power of 10, per IEEE spec.  */
  while ((nexp > 0) && (yy[2] == 0))
    {
      emovz (yy, xt);
      eshup1 (xt);
      eshup1 (xt);
      eaddm (yy, xt);
      eshup1 (xt);
      if (xt[2] != 0)
	break;
      nexp -= 1;
      emovz (xt, yy);
    }
  if ((k = enormlz (yy)) > NBITS)
    {
      ecleaz (yy);
      goto aexit;
    }
  lexp = (EXONE - 1 + NBITS) - k;
  emdnorm (yy, lost, 0, lexp, 64);
  lost = 0;

  /* Convert to external format:

     Multiply by 10**nexp.  If precision is 64 bits,
     the maximum relative error incurred in forming 10**n
     for 0 <= n <= 324 is 8.2e-20, at 10**180.
     For 0 <= n <= 999, the peak relative error is 1.4e-19 at 10**947.
     For 0 >= n >= -999, it is -1.55e-19 at 10**-435.  */

  lexp = yy[E];
  if (nexp == 0)
    {
      k = 0;
      goto expdon;
    }
  esign = 1;
  if (nexp < 0)
    {
      nexp = -nexp;
      esign = -1;
      if (nexp > 4096)
	{
	  /* Punt.  Can't handle this without 2 divides.  */
	  emovi (etens[0], tt);
	  lexp -= tt[E];
	  k = edivm (tt, yy);
	  lexp += EXONE;
	  nexp -= 4096;
	}
    }
  p = &etens[NTEN][0];
  emov (eone, xt);
  exp = 1;
  do
    {
      if (exp & nexp)
	emul (p, xt, xt);
      p -= NE;
      exp = exp + exp;
    }
  while (exp <= MAXP);

  emovi (xt, tt);
  if (esign < 0)
    {
      lexp -= tt[E];
      k = edivm (tt, yy);
      lexp += EXONE;
    }
  else
    {
      lexp += tt[E];
      k = emulm (tt, yy);
      lexp -= EXONE - 1;
    }
  lost = k;

 expdon:

  /* Round and convert directly to the destination type */
  if (oprec == 53)
    lexp -= EXONE - 0x3ff;
  else if (oprec == 24)
    lexp -= EXONE - 0177;
  rndprc = oprec;
  emdnorm (yy, lost, 0, lexp, 64);

 aexit:

  rndprc = rndsav;
  yy[0] = nsign;
  switch (oprec)
    {

    case 53:
      toe53 (yy, y);
      break;
    case 24:
      toe24 (yy, y);
      break;
    case NBITS:
      emovo (yy, y);
      break;
    }
}



/* Return Y = largest integer not greater than X (truncated toward minus
   infinity).  */

static uint16_t bmask[] =
{
  0xffff,
  0xfffe,
  0xfffc,
  0xfff8,
  0xfff0,
  0xffe0,
  0xffc0,
  0xff80,
  0xff00,
  0xfe00,
  0xfc00,
  0xf800,
  0xf000,
  0xe000,
  0xc000,
  0x8000,
  0x0000,
};

static void
efloor (x, y)
     uint16_t x[], y[];
{
  register uint16_t *p;
  int e, expon, i;
  uint16_t f[NE];

  emov (x, f);			/* leave in external format */
  expon = (int) f[NE - 1];
  e = (expon & 0x7fff) - (EXONE - 1);
  if (e <= 0)
    {
      eclear (y);
      goto isitneg;
    }
  /* number of bits to clear out */
  e = NBITS - e;
  emov (f, y);
  if (e <= 0)
    return;

  p = &y[0];
  while (e >= 16)
    {
      *p++ = 0;
      e -= 16;
    }
  /* clear the remaining bits */
  *p &= bmask[e];
  /* truncate negatives toward minus infinity */
 isitneg:

  if ((uint16_t) expon & (uint16_t) 0x8000)
    {
      for (i = 0; i < NE - 1; i++)
	{
	  if (f[i] != y[i])
	    {
	      esub (eone, y, y);
	      break;
	    }
	}
    }
}



/* Return e type Y = X * 2^PWR2.  */

static void
eldexp (x, pwr2, y)
     uint16_t x[];
     int pwr2;
     uint16_t y[];
{
  uint16_t xi[NI];
  int32_t li;
  int i;

  emovi (x, xi);
  li = xi[1];
  li += pwr2;
  i = 0;
  emdnorm (xi, i, i, li, 64);
  emovo (xi, y);
}



/*  Return quotient of exploded e-types NUM / DEN in EQUOT,
    remainder in NUM.  */

static void
eiremain (den, num)
     uint16_t den[], num[];
{
  int32_t ld, ln;
  uint16_t j;

  ld = den[E];
  ld -= enormlz (den);
  ln = num[E];
  ln -= enormlz (num);
  ecleaz (equot);
  while (ln >= ld)
    {
      if (ecmpm (den, num) <= 0)
	{
	  esubm (den, num);
	  j = 1;
	}
      else
	  j = 0;
      eshup1 (equot);
      equot[NI - 1] |= j;
      eshup1 (num);
      ln -= 1;
    }
  emdnorm (num, 0, 0, ln, 0);
}

/* Report an error condition CODE encountered in function NAME.
   CODE is one of the following:

    Mnemonic        Value          Significance

     DOMAIN            1       argument domain error
     SING              2       function singularity
     OVERFLOW          3       overflow range error
     UNDERFLOW         4       underflow range error
     TLOSS             5       total loss of precision
     PLOSS             6       partial loss of precision
     INVALID           7       NaN - producing operation
     EDOM             33       Unix domain error code
     ERANGE           34       Unix range error code

   The order of appearance of the following messages is bound to the
   error codes defined above.  */

#define NMSGS 8
static char *ermsg[NMSGS] =
{
  "unknown",			/* error code 0 */
  "domain error",		/* error code 1 */
  "singularity",		/* et seq.      */
  "overflow",
  "underflow",
  "total loss of precision",
  "partial loss of precision",
  "`not-a-number' produced"
};

int merror = 0;
extern int merror;

static void
mtherr (name, code)
     char *name;
     int code;
{
  char errstr[80];

  /* The string passed by the calling program is supposed to be the
     name of the function in which the error occurred.
     The code argument selects which error message string will be printed.  */

  if ((code <= 0) || (code >= NMSGS))
    code = 0;
  if (strcmp (name, "esub") == 0)
    name = "subtraction";
  else if (strcmp (name, "ediv") == 0)
    name = "division";
  else if (strcmp (name, "emul") == 0)
    name = "multiplication";
  else if (strcmp (name, "enormlz") == 0)
    name = "normalization";
  else if (strcmp (name, "etoasc") == 0)
    name = "conversion to text";
  else if (strcmp (name, "asctoe") == 0)
    name = "parsing";
  else if (strcmp (name, "eremain") == 0)
    name = "modulus";
  else if (strcmp (name, "esqrt") == 0)
    name = "square root";
  sprintf (errstr, "%s during real %s", ermsg[code], name);
  if (extra_warnings)
    warning (errstr);
  /* Set global error message word */
  merror = code + 1;
}





/* Output a binary NaN bit pattern in the target machine's format.  */

uint16_t DFbignan[4] = {0x7fff, 0xffff, 0xffff, 0xffff};
uint16_t DFlittlenan[4] = {0, 0, 0, 0xfff8};

uint16_t SFbignan[2] = {0x7fff, 0xffff};
uint16_t SFlittlenan[2] = {0, 0xffc0};


static void
make_nan (nan, sign, mode)
     uint16_t *nan;
     int sign;
     enum machine_mode mode;
{
  int n;
  uint16_t *p;

  switch (mode)
    {
    case DFmode:
      n = 4;
      if (REAL_WORDS_BIG_ENDIAN)
	p = DFbignan;
      else
	p = DFlittlenan;
      break;

    case SFmode:
      n = 2;
      if (REAL_WORDS_BIG_ENDIAN)
	p = SFbignan;
      else
	p = SFlittlenan;
      break;

    default:
      abort ();
    }
  if (REAL_WORDS_BIG_ENDIAN)
    *nan++ = (sign << 15) | (*p++ & 0x7fff);
  while (--n != 0)
    *nan++ = *p++;
  if (! REAL_WORDS_BIG_ENDIAN)
    *nan = (sign << 15) | (*p & 0x7fff);
}

/* This is the inverse of the function `etarsingle' invoked by
   REAL_VALUE_TO_TARGET_SINGLE.  */

REAL_VALUE_TYPE
ereal_unto_float (f)
     long f;
{
  REAL_VALUE_TYPE r;
  uint16_t s[2];
  uint16_t e[NE];

  /* Convert 32 bit integer to array of 16 bit pieces in target machine order.
   This is the inverse operation to what the function `endian' does.  */
  if (REAL_WORDS_BIG_ENDIAN)
    {
      s[0] = (uint16_t) (f >> 16);
      s[1] = (uint16_t) f;
    }
  else
    {
      s[0] = (uint16_t) f;
      s[1] = (uint16_t) (f >> 16);
    }
  /* Convert and promote the target float to E-type. */
  e24toe (s, e);
  /* Output E-type to REAL_VALUE_TYPE. */
  PUT_REAL (e, &r);
  return r;
}


/* This is the inverse of the function `etardouble' invoked by
   REAL_VALUE_TO_TARGET_DOUBLE.  */

REAL_VALUE_TYPE
ereal_unto_double (d)
     long d[];
{
  REAL_VALUE_TYPE r;
  uint16_t s[4];
  uint16_t e[NE];

  /* Convert array of HOST_WIDE_INT to equivalent array of 16-bit pieces.  */
  if (REAL_WORDS_BIG_ENDIAN)
    {
      s[0] = (uint16_t) (d[0] >> 16);
      s[1] = (uint16_t) d[0];
      s[2] = (uint16_t) (d[1] >> 16);
      s[3] = (uint16_t) d[1];
    }
  else
    {
      /* Target float words are little-endian.  */
      s[0] = (uint16_t) d[0];
      s[1] = (uint16_t) (d[0] >> 16);
      s[2] = (uint16_t) d[1];
      s[3] = (uint16_t) (d[1] >> 16);
    }
  /* Convert target double to E-type. */
  e53toe (s, e);
  /* Output E-type to REAL_VALUE_TYPE. */
  PUT_REAL (e, &r);
  return r;
}


/* Convert an SFmode target `float' value to a REAL_VALUE_TYPE.
   This is somewhat like ereal_unto_float, but the input types
   for these are different.  */

REAL_VALUE_TYPE
ereal_from_float (f)
     HOST_WIDE_INT f;
{
  REAL_VALUE_TYPE r;
  uint16_t s[2];
  uint16_t e[NE];

  /* Convert 32 bit integer to array of 16 bit pieces in target machine order.
   This is the inverse operation to what the function `endian' does.  */
  if (REAL_WORDS_BIG_ENDIAN)
    {
      s[0] = (uint16_t) (f >> 16);
      s[1] = (uint16_t) f;
    }
  else
    {
      s[0] = (uint16_t) f;
      s[1] = (uint16_t) (f >> 16);
    }
  /* Convert and promote the target float to E-type.  */
  e24toe (s, e);
  /* Output E-type to REAL_VALUE_TYPE.  */
  PUT_REAL (e, &r);
  return r;
}


/* Convert a DFmode target `double' value to a REAL_VALUE_TYPE.
   This is somewhat like ereal_unto_double, but the input types
   for these are different.

   The DFmode is stored as an array of HOST_WIDE_INT in the target's
   data format, with no holes in the bit packing.  The first element
   of the input array holds the bits that would come first in the
   target computer's memory.  */

REAL_VALUE_TYPE
ereal_from_double (d)
     HOST_WIDE_INT d[];
{
  REAL_VALUE_TYPE r;
  uint16_t s[4];
  uint16_t e[NE];

  /* Convert array of HOST_WIDE_INT to equivalent array of 16-bit pieces.  */
  if (REAL_WORDS_BIG_ENDIAN)
    {
      s[0] = (uint16_t) (d[0] >> 16);
      s[1] = (uint16_t) d[0];
#if HOST_BITS_PER_WIDE_INT == 32
      s[2] = (uint16_t) (d[1] >> 16);
      s[3] = (uint16_t) d[1];
#else
      /* In this case the entire target double is contained in the
	 first array element.  The second element of the input is
	 ignored.  */
      s[2] = (uint16_t) (d[0] >> 48);
      s[3] = (uint16_t) (d[0] >> 32);
#endif
    }
  else
    {
      /* Target float words are little-endian.  */
      s[0] = (uint16_t) d[0];
      s[1] = (uint16_t) (d[0] >> 16);
#if HOST_BITS_PER_WIDE_INT == 32
      s[2] = (uint16_t) d[1];
      s[3] = (uint16_t) (d[1] >> 16);
#else
      s[2] = (uint16_t) (d[0] >> 32);
      s[3] = (uint16_t) (d[0] >> 48);
#endif
    }
  /* Convert target double to E-type.  */
  e53toe (s, e);
  /* Output E-type to REAL_VALUE_TYPE.  */
  PUT_REAL (e, &r);
  return r;
}


/* Return the binary precision of the significand for a given
   floating point mode.  The mode can hold an integer value
   that many bits wide, without losing any bits.  */

int
significand_size (mode)
     enum machine_mode mode;
{

/* Don't test the modes, but their sizes, lest this
   code won't work for BITS_PER_UNIT != 8 .  */

switch (GET_MODE_BITSIZE (mode))
  {
  case 32:
    return 24;
  case 64:
    return 53;
  default:
    abort ();
  }
}
