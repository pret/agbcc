/*
From: uunet!mcvax!sor.inria.fr!tiemann (Mike Tiemann)
Date: Sun, 4 Sep 88 11:15:27 +0100

I got all your tests working (except str1597a.c), and the compiler
bootstrapping itself, and I thought I was done.  Then I remembered
*my* favorite test program...which used to work, but now fails again.
Here it is, for your collecting joy:

[Altered in obvious ways to avoid using function prototypes -- gnu]
*/

/* a reasonably sized structure.  */
typedef struct foo
{
  int x[57];
} foo;

int bad = 0;

foo sum (x, y)
  foo x; foo y;
{
  foo s;
  int i;

  for (i = 0; i < 57; i++)
    {
      if (x.x[i] != 1) {printf("sum x[%d] = %d\n", i, x.x[i]); bad++;}
      if (y.x[i] != 2) {printf("sum y[%d] = %d\n", i, y.x[i]); bad++;}
      s.x[i] = x.x[i] + y.x[i];
    }
  return s;
}

foo init (val)
  int val;
{
  foo s;
  int i;

  for (i = 0; i < 57; i++)
    s.x[i] = val;
  return s;
}

main ()
{
  foo s;
  foo t;
  int i;

  s = sum (init (1), init (2));
  t = sum (init (1), init (2));

  for (i = 0; i < 57; i++)
    if (s.x[i] != 3 || t.x[i] != 3) {
      printf ("failure at %i\n", i);
      bad++;
    }
  if (bad) printf ("Failed %d ways.\n", bad);
  else     printf ("Test passed.\n");
}
