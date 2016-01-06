main() {
	struct s {unsigned i9:9, i1:1;} x;
	x.i9 = 0;
	x.i1 = 3;		/* Note: larger than bitfield */
	if ( x.i9 == 0 )
		printf("Test passed\n");
	else
		printf("Failed...\n");
}
