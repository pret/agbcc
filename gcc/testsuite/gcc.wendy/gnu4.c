struct s {int x[5];};
main()
{
        struct s {struct s *next; int i;};
	static struct s sa[2];
 
        /* bar(sa[0].next->x[4]); */
	printf("Test passed (if it compiled)\n");
	exit(0);
        bar(sa[0].next->i);
}

bar(i)
int i;
{}
