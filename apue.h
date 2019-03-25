/*
 * 이 책의 헤더 파일. 응용 프로그램은 모든 표준 시스템 헤더들을
 * 포함시키기 전에 이 헤더를 포함시켜야 한다.
 */
#ifndef _APUE_H
#define _APUE_H

#define _POSIX_C_SOURCE 200809L

#if defined(SOLARIS)		/* Solaris 10 */
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 700
#endif

#include <sys/types.h>		/* 이 헤더가 필요한 시스템들이 아직 있다 */
#include <sys/stat.h>
#include <sys/termios.h>	/* winsize를 위해 */
#if defined(MACOS) || !defined(TIOCGWINSZ)
#include <sys/ioctl.h>
#endif

#include <stdio.h>			/* 편의를 위해 */
#include <stdlib.h>			/* 편의를 위해 */
#include <stddef.h>			/* offsedof를 위해*/
#include <string.h>			/* 편의를 위해 */
#include <unistd.h>			/* 편의를 위해 */
#include <signal.h>			/* SIG_ERR를 위해 */

#define MAXLINE 4096		/* 한 줄의 최대 길이 */

/*
 * 새 파일에 적용할 기본 파일 접근 권한들.
 */
#define FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/*
 * 새 디렉터리에 적용할 기본 접근 권한들.
 */
#define DIR_MODE	(FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

typedef void	Sigfunc(int);	/* 신호 처리부 함수의 형식 */

#define min(a, b)	((a) < (b) ? (a) : (b))
#define max(a, b)	((a) > (b) ? (a) : (b))

/*
 * 이 책에서 만든 함수들의 원형들
 */
char	*path_alloc(size_t *);				/* 도해 2.16 */
long	open_max(void);						/* 도해 2.17 */

int		set_cloexec(int);					/* 도해 13.9 */
void 	clr_fl(int, int);				
void 	set_fl(int, int);					/* 도해 3.12 */

void 	pr_exit(int);						/* 도해 8.5 */

void 	pr_mask(const char *);				/* 도해 10.14 */
Sigfunc	*signal_intr(int, Sigfunc *);		/* 도해 10.19 */

void	daemonize(const char *);			/* 도해 13.1 */

void	sleep_us(unsigned int);				/* 연습문제 14.5 */
ssize_t	readn(int, void *, size_t);			/* 도해 14.24 */
ssize_t	writen(int, const void *, size_t);	/* 도해 14.24 */

int 	fd_pipe(int *);						/* 도해 17.2 */
int 	recv_fd(int, ssize_t (*func)(int, 
				const void *, size_t));		/* 도해 17.14 */
int 	send_fd(int, int);					/* 도해 17.13 */
int 	send_err(int, int, const char *);	/* 도해 17.12 */

int 	serv_listen(const char *);			/* 도해 17.8 */
int 	serv_accept(int, uid_t *);			/* 도해 17.9 */
int 	cli_conn(const char *);				/* 도해 17.10 */
int 	buf_args(char *, int (*func)(int,
				char **));					/* 도해 17.23 */

int 	tty_cbreak(int);					/* 도해 18.20 */
int 	tty_raw(int);						/* 도해 18.20 */
int 	tty_reset(int);						/* 도해 18.20 */
int 	tty_atexit(void);					/* 도해 18.20 */
struct termios	*tty_termios(void);			/* 도해 18.20 */

int 	ptym_open(char *, int);				/* 도해 19.9 */
int 	ptys_open(char *);					/* 도해 19.9 */
#ifdef TIOCGWINSZ
pid_t	pty_fork(int *, char *, int, const struct termios *, 
				const struct winsize *);	/* 도해 19.10 */
#endif	

int 	lock_reg(int, int, int, off_t, int, off_t);	/* 도해 14.5 */

#define read_lock(fd, offset, whence, len) \
			lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
#define readw_lock(fd, offset, whence, len) \
			lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
#define write_lock(fd, offset, whence, len) \
			lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
#define writew_lock(fd, offset, whence, len) \
			lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
#define un_lock(fd, offset, whence, len) \
			lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))

pid_t	lock_test(int, int, off_t, int, off_t);		/* 도해 14.6 */ 

#define is_read_lockable(fd, offset, whence, len) \
			(lock_test((fc), F_RDLCK, (offset), (whence), (len)) == 0)
#define is_write_lockable(fd, offset, whence, len) \
			(lock_test((fc), F_WRLCK, (offset), (whence), (len)) == 0)	

void	err_msg(const char *, ...);			/* 부록 B */
void	err_dump(const char *, ...) __attribute__((noreturn));
void 	err_quit(const char *, ...) __attribute__((noreturn));
void	err_cont(int, const char *, ...);
void 	err_exit(int, const char *, ...) __attribute__((noreturn));
void	err_ret(const char *, ...);
void	err_sys(const char *, ...) __attribute__((noreturn));

void	log_msg(const char *, ...);			/* 부록 B */
void 	log_open(const char *, int, int);
void	log_quit(const char *, ...) __attribute__((noreturn));
void 	log_ret(const char *, ...);
void 	log_sys(const char *, ...) __attribute__((noreturn));
void	log_exit(int, const char *, ...) __attribute__((noreturn));

void	TELL_WAIT(void);	/* 8.9의 부모/자식 동기화 루틴들 */
void 	TELL_PARENT(pid_t);
void 	TELL_CHILD(pid_t);
void	WAIT_PARENT(void);
void 	WAIT_CHILD(void);

#endif	/* _APUE_H */













