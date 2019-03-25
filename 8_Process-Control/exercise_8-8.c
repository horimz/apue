#include "apue.h"
#include <sys/wait.h>

int
main(void)
{
    pid_t   pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {      /* 첫째 자식 */
        if ((pid = fork()) < 0)
            err_sys("fork error");
        else if (pid > 0)
            exit(0);            /* 두 번째 fork의 부모 == 첫째 자식 */
        
        /*
         * 이 부분은 둘째 자식이다. 기존 부모가 위의 exit()를 호출하면
         * init 프로세스가 이 프로세스의 부모가 된다.
         * 이 프로세스가 종료되면 init이 이 프로세스의 상태를 회수할
         * 것이므로 이런 식으로 실행을 계속해도 무방하다.
         */ 
        sleep(2);
        printf("second child, parent pid = %ld\n", (long)getppid());
        exit(0);
    }
    if (waitpid(pid, NULL, 0) != pid)   /* 첫 자식 프로세스를 기다린다. */
        err_sys("waitpid error");

    /*
     * 이 부분은 부모 프로세스(원래의 프로세스)이다. 이 프로세스는 둘째 자식의
     * 부모가 아니므로 그대로 실행을 진행해도 무방하다.
     */
    exit(0); 
}