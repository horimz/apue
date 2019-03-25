#include "apue.h"

static void sig_alrm(int);

int
main(void)
{
    int     n;
    char    line[MAXLINE];

    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        err_sys("signal(SIGALRM) error");
    
    alarm(10);
    if ((n = read(STDERR_FILENO, line, MAXLINE)) < 0)
        err_sys("read error");
    alarm(0);

    write(STDOUT_FILENO, line, n);
    exit(0);
}

static void 
sig_alrm(int signo)
{
    /* 아무 것도 하지 않고 그냥 반환(이에 의해 read가 가로채인다) */
}