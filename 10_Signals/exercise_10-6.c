#include "apue.h"
#include <sys/wait.h>

static void sig_cld(int);

int
main()
{
    pid_t   pid;
    
    if (signal(SIGCLD, sig_cld) == SIG_ERR)
        perror("signal error");
    if ((pid = fork()) < 0 ) {
        perror("fork error");
    } else if (pid == 0) {      /* 자식 */
        sleep(2);
        _exit(0);
    }

    pause();                    /* 부모 */
    exit(0);
}

static void 
sig_cld(int signo)      /* pause()를 가로챈다 */
{
    pid_t   pid;
    int     status;

    printf("SIGCLD received\n");

    if (signal(SIGCLD, sig_cld) == SIG_ERR) /* 처리부를 재설정 */
        perror("signal error");
    
    if ((pid = wait(&status)) < 0)          /* 자식 상태를 회수 */
        perror("wait error");
    
    printf("pid = %d\n", pid);
}