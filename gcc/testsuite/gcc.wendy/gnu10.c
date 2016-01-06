/*
 * Unsigned bitfields now promote to signed ints in expressions, grumble.
 */
main() {
	struct s {unsigned i9:9, i1:1;} x;
	x.i9 = 0;
	if ( x.i9 <= -139 )
		printf("Failed...\n");
	else
		printf("Test passed\n");
}
