static int it1, it3;
int i1 = 4, i2 = 7;
main() {
	if ( (it3 = (it1 = i1) + (it1 = i2)) == i1+i2 )
		printf("Test passed\n");
	else
		printf("Failed...\n");
}
