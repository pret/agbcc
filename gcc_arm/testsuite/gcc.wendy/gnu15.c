/*
Date: Sun, 5 Jul 87 18:24:54 PDT
From: andy@csvax.caltech.edu (Andy Fyfe)
To: bug-gcc@prep.ai.mit.edu
Subject: unsigned -> float conversion
*/

main()
{
    unsigned u = 0x80000000;

    printf("(f)u = %g\n", (float)u);
    printf("(f)(u)0x80000000 = %g\n", (float)(unsigned)0x80000000);

    exit(0);
}

/*
Using 68020/68881 the above gives:

(f)u = 2.14748e+09
(f)(u)0x80000000 = -2.14748e+09

Using 68020/soft-float the above gives:

(f)u = -2.14748e+09
(f)(u)0x80000000 = -2.14748e+09

gcc, version 1.6, compiled on a sun3 by gcc itself.
*/
