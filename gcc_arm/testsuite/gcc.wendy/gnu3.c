/*
 * From ptsfa!ames!hc!siegel Tue Nov 24 15:38:28 1987
 * From: siegel@hc.DSPO.GOV (Josh Siegel)
 * Newsgroups: comp.bugs.4bsd
 * Subject: Bug in Sun compiler...
 * Message-ID: <11636@hc.DSPO.GOV>
 * Date: 24 Nov 87 23:38:28 GMT
 * Organization: Los Alamos National Laboratory
 * 
 * The program causes Suns compiler to generate code that 
 * has a Segmentation fault upon exiting.
 * 
 * The reason being that Sun don't check to see if you specificly 
 * returned a structure.  The register d0 is null and it offsets 
 * through it.
 * 
 * Vax and GNU do this correctly...
 */

struct {
        int a,b,c;
        } 
bar()
{}

main()
{
	bar();
	printf("Test passed\n");
}
