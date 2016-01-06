// Build don't link:
// excess errors test - XFAIL *-linux-gnu

char *a="aê";

class A
{
public:
	A()
	{
		char *b="aê";
	}
};

char *c="aê";
