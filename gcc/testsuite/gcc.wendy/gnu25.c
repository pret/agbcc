/*
 * gnu25.c
 *
 * This was originally reported as a bug in the Sun C compiler in
 * Sun-Spots, by ihnp4!stcvax!stc-auts!kak, but he forgot to declare
 * double sqrt().  However, the code generated for the initialization
 * by gcc-1.19 plus a fix, reuses fp0 for both arguments of the divide.
 * oops!  the result is always 1!

	John Gilmore, 16April88
 * 
 * showbug
 * compile: cc showbug.c -o showbug -lm
 */
double sqrt();
compare(a, b)
	double a, b;
{

	if (a != b) return 1;		/* Not computed the same */
	if (a > 0.014) return 1;	/* Wrong answer */
	if (a < 0.012) return 1;	/* Wrong answer */
	return 0;
}
main()
{
        float mean = 0.035000;
        int samples = 200;
        {
                double sigma = sqrt( (mean*(1 - mean))/ samples);

		if (compare(sigma, 
                        sqrt((mean*(1 - mean))/ samples ))) {

			printf("sigma = sqrt( (%f)/%d) ",
				(mean*(1 - mean)), samples );
			printf("= sqrt( %f ) ",
				(mean*(1 - mean))/ samples );
			printf("= %f ",
				sqrt((mean*(1 - mean))/ samples ));
			printf("= %f (!)\n", sigma );
		} else {
			printf("Test passed\n");
		}
        }
}

/* We'd like to link with -lm, but "runt" doesnt do this. */
double sqrt(x) double x;
{
	/* Quick fakery. */
	if (x > .000165 && x < .00017)	 /* arg is right */
		return .0129951914;		/* Right result */
	return 0;		/* CHeap imitation sqrt routine */
}
