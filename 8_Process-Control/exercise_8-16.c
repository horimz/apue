#include "apue.h"
#include <sys/wait.h>

char    *env_init[] = { "USER=unknown", "PATH=tmp", NULL };

int
main(void)
{
    pid_t   pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {      /* 경로이름과 환경을 지정해서 실행 */
        if (execle("./echoall", "echoall", "myarg1",        /* 편의를 위해 경로를 교재와 다르게 설정하였음 */
                    "MY ARG2", (char *)0, env_init) < 0)
            err_sys("execle error");
    } 

    if (waitpid(pid, NULL, 0) < 0)
        err_sys("wait error");
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {      /* 파일이름은지정하고 환경은 물려준다 */
        if (execlp("echoall", "echoall", "only 1 arg", (char *)0) < 0)
            err_sys("execlp error");
    }

    exit(0);
}