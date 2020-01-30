/*
 * Causes gnu cc 1.14 to coredump.  Not sure why.
 * An excerpt from PD tar's "list.c" file.
 */
typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;
typedef	unsigned short	ushort;		 
typedef	unsigned int	uint;		 








typedef	struct	_physadr { short r[1]; } *physadr;
typedef	struct	label_t	{
	int	val[13];
} label_t;

typedef	struct	_quad { long val[2]; } quad;
typedef	long	daddr_t;
typedef	char *	caddr_t;
typedef	u_long	ino_t;
typedef	long	swblk_t;

typedef	long	time_t;
typedef	short	dev_t;
typedef	int	off_t;
typedef long	key_t;

typedef	struct	fd_set { int fds_bits[1]; } fd_set;


struct	stat {
	dev_t	st_dev;
	ino_t	st_ino;
	unsigned short st_mode;
	short	st_nlink;
	short	st_uid;
	short	st_gid;
	dev_t	st_rdev;
	off_t	st_size;
	time_t	st_atime;
	int	st_spare1;
	time_t	st_mtime;
	int	st_spare2;
	time_t	st_ctime;
	int	st_spare3;
	long	st_blksize;
	long	st_blocks;
	long	st_spare4[2];
};

union record {
	char		charptr[512 ];
	struct header {
		char	name[100 ];
		char	mode[8];
		char	uid[8];
		char	gid[8];
		char	size[12];
		char	mtime[12];
		char	chksum[8];
		char	linkflag;
		char	linkname[100 ];
		char	magic[8];
		char	uname[32 ];
		char	gname[32 ];
		char	devmajor[8];
		char	devminor[8];
	} header;
};
long from_oct();			 

decode_header(header, st, stdp, wantug)
	register union record	*header;
	register struct stat	*st;
	int	*stdp;
	int	wantug;
{

	st->st_mode = from_oct(8,  header->header.mode);
	st->st_mtime = from_oct(1+12, header->header.mtime);
	
	if (0==strcmp(header->header.magic, 	"ustar  "	)) {
		 
		*stdp = 1;
		if (wantug) {
			st->st_uid = finduid(header->header.uname);
			st->st_gid = findgid(header->header.gname);
		}
		switch  (header->header.linkflag) 
		case 	'4'		: case 	'3'		:
		    st->st_rdev = ((dev_t)(((from_oct(8, header->header.devmajor))<<8) | (
			 		  from_oct(8, header->header.devminor)))) ;
	} else {
		*stdp = 0;
		st->st_uid = from_oct(8,  header->header.uid);
		st->st_gid = from_oct(8,  header->header.gid);
		st->st_rdev = 0;
	}
}

main(){
	printf("Test passed (if it compiled at all)\n");
}

long from_oct(x)
	char *x;
{
	return 0;
}

int finduid(x) char *x;
{	return 0; }

int findgid(x) char *x;
{	return 0; }
