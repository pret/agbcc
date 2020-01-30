unsigned char	msgi[10] = {0x10, 0x09, 0x88, 0xaa, 0x22, 0x09};
#define	MAGIC	0125252		/* checksum is subtracted from this */

int
main()
{
	unsigned short pktsum, oursum;

	pktsum = msgi[2] + (msgi[3] << 8);

	/* Check checksums for control packets */
	oursum = MAGIC - msgi[4];
	if (pktsum != oursum)
		printf("control checksum bad %x %x\n", pktsum, oursum);
	else
		printf("Test passed\n");
}
