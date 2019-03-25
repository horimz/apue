#include "apue.h"
#include <errno.h>

static void
sig_hup(int signo)
{
    printf("SIGHUP received, pid = %ld\n", (long)getpid());
}
static void
pr_ids(char *name)
{
    printf("%s: pid = %ld, ppid = %ld, pgrp = %ld, tpgrp = %ld\n",
        name, (long)getpid(), (long)getppid(), (long)getpgrp(),
        (long)tcgetpgrp(STDIN_FILENO));
    fflush(stdout);
}
int
main(void)
{
    char    c;
    pid_t   pid;
    pr_ids("parent");
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {   /* 부모 */
        sleep(5);       /* 자식이 스스로 정지하도록 잠시 수면에 빠진다 */
    } else {            /* 자식 */
        pr_ids("child");
        signal(SIGHUP, sig_hup);        /* 신호 처리부를 설정한다 */
        kill(getpid(), SIGTSTP);        /* 스스로를 정지시킨다 */
        pr_ids("child");        /* 실행 재개 */
        if (read(STDIN_FILENO, &c, 1) != 1)
            printf("read error %d on controlling TTY\n", errno);
    }
    exit(0);
}
