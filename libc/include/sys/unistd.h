#ifndef _SYS_UNISTD_H
#define _SYS_UNISTD_H

#ifdef __cplusplus
extern "C" {
#endif


#include <sys/types.h>
#define __need_size_t
#include <stddef.h>

extern char **environ;

void _exit(int __status) __attribute__((noreturn));

int access(const char *__path, int __amode);
unsigned alarm(unsigned __secs);
int chdir(const char *__path);
int chmod(const char *__path, mode_t __mode);
int chown(const char *__path, uid_t __owner, gid_t __group);
int close(int __fildes);
char *ctermid(char *__s);
char *cuserid(char *__s);
int dup(int __fildes);
int dup2(int __fildes, int __fildes2);
int execl(const char *__path, const char *, ...);
int execle(const char *__path, const char *, ...);
int execlp(const char *__file, const char *, ...);
int execv(const char *__path, char *const __argv[]);
int execve(const char *__path, char *const __argv[], char *const __envp[]);
int execvp(const char *__file, char *const __argv[]);
pid_t fork(void);
long fpathconf(int __fd, int __name);
int fsync(int __fd);
char *getcwd(char *__buf, size_t __size);
gid_t getegid(void);
uid_t geteuid(void);
gid_t getgid(void);
int getgroups(int __gidsetsize, gid_t __grouplist[]);
char *getlogin(void);
char *getpass(__const char *__prompt);
size_t getpagesize(void);
pid_t getpgrp(void);
pid_t getpid(void);
pid_t getppid(void);
uid_t getuid(void);
int isatty(int __fildes);
int link(const char *__path1, const char *__path2);
off_t lseek(int __fildes, off_t __offset, int __whence);
long pathconf(char *__path, int __name);
int pause(void);
int pipe(int __fildes[2]);
int read(int __fildes, void *__buf, size_t __nbyte);
int rmdir(const char *__path);
void *sbrk(size_t __incr);
int setgid(gid_t __gid);
int setpgid(pid_t __pid, pid_t __pgid);
pid_t setsid(void);
int setuid(uid_t __uid);
unsigned sleep(unsigned int __seconds);
void swab(const void *, void *, ssize_t);
long sysconf(int __name);
pid_t tcgetpgrp(int __fildes);
int tcsetpgrp(int __fildes, pid_t __pgrp_id);
char *ttyname(int __fildes);
int unlink(const char *__path);
int write(int __fildes, const void *__buf, size_t __nbyte);

/* Provide prototypes for most of the _<systemcall> names that are
   provided in newlib for some compilers.  */
int _close(int __fildes);
pid_t _fork(void);
pid_t _getpid(void);
int _link(const char *__path1, const char *__path2);
off_t _lseek(int __fildes, off_t __offset, int __whence);
int _read(int __fildes, void *__buf, size_t __nbyte);
void *_sbrk(size_t __incr);
int _unlink(const char *__path);
int _write(int __fildes, const void *__buf, size_t __nbyte);

#ifdef __CYGWIN32__
unsigned usleep(unsigned int __useconds);
int ftruncate(int __fd, off_t __length);
int truncate(const char *, off_t __length);
int gethostname(char *__name, size_t __len);
char *mktemp(char *);
int sync(void);
int readlink(const char *__path, char *__buf, int __buflen);
int symlink(const char *__name1, const char *__name2);
#endif

#define F_OK 0
#define R_OK 4
#define W_OK 2
#define X_OK 1

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#ifdef __svr4__
#define _POSIX_JOB_CONTROL 1
#define _POSIX_SAVED_IDS 1
#define _POSIX_VERSION 199009L
#endif

#ifdef __CYGWIN32__
#define _POSIX_JOB_CONTROL 1
#define _POSIX_SAVED_IDS 0
#define _POSIX_VERSION 199009L
#endif

#define STDIN_FILENO 0  /* standard input file descriptor */
#define STDOUT_FILENO 1 /* standard output file descriptor */
#define STDERR_FILENO 2 /* standard error file descriptor */

long sysconf(int __name);

#define _SC_ARG_MAX 0
#define _SC_CHILD_MAX 1
#define _SC_CLK_TCK 2
#define _SC_NGROUPS_MAX 3
#define _SC_OPEN_MAX 4
/* no _SC_STREAM_MAX */
#define _SC_JOB_CONTROL 5
#define _SC_SAVED_IDS 6
#define _SC_VERSION 7
#define _SC_PAGESIZE 8

#define _PC_LINK_MAX 0
#define _PC_MAX_CANON 1
#define _PC_MAX_INPUT 2
#define _PC_NAME_MAX 3
#define _PC_PATH_MAX 4
#define _PC_PIPE_BUF 5
#define _PC_CHOWN_RESTRICTED 6
#define _PC_NO_TRUNC 7
#define _PC_VDISABLE 8

#ifndef _POSIX_SOURCE
#define MAXNAMLEN 1024
#endif /* _POSIX_SOURCE */

/* FIXME: This is temporary until winsup gets sorted out.  */
#ifdef __CYGWIN32__
#define MAXPATHLEN (260 - 1 /* NUL */)
#else
#define MAXPATHLEN 1024
#endif

#ifdef __cplusplus
}
#endif
#endif /* _SYS_UNISTD_H */
