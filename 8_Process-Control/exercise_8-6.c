#include "apue.h"
#include <sys/wait.h>

int
main(void)
{
    pid_t   pid;
    int     status;

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0)                  /* 자식 */
        exit(7);

    if (wait(&status) != pid)           /* 자식의 종료를 기다리고 */
        err_sys("wait error");
    pr_exit(status);                    /* 종료 상태를 출력한다. */
    /*---------------------------------------------------------------------*/
    if ((pid = fork() < 0))
        err_sys("fork error");
    else if (pid == 0)                  /* 자식 */
        abort();                        /* SIGABRT 신호를 발생 */
        
    if (wait(&status) != pid)           /* 자식의 종료를 기다리고 */
        err_sys("wait error");
    pr_exit(status);                    /* 종료 상태를 출력한다. */
    /*---------------------------------------------------------------------*/
    if ((pid = fork() < 0))
        err_sys("fork error");
    else if (pid == 0)                  /* 자식 */
        status /= 0;                    /* 0으로 나누기에 의해 SIGFPE가 발생 */
        
    if (wait(&status) != pid)           /* 자식의 종료를 기다리고 */
        err_sys("wait error");
    pr_exit(status);                    /* 종료 상태를 출력한다. */
    
    exit(0);
}