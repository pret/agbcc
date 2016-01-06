#define Neg -113
static unsigned int ui2 = Neg;
main() {
	if ( ui2%2 == ((unsigned)Neg)%2 )
		printf("Test passed\n");
	else
		printf("Failed...\n");
}
