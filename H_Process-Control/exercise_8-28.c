#include "apue.h"

int
main(void)
{
    pid_t   pid;

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid != 0) {            /* 부모 */
        sleep(2);
        exit(2);                    /* 종료 상태 2로 종료됨 */
    }
    
    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid != 0) {            /* 첫째 자식 */
        sleep(4);
        abort();                    /* 코어를 덤프하고 종료됨 */
    }

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid != 0) {            /* 둘째 자식 */
        execl("/bin/dd", "dd", "if=/etc/passwd", "of=/dev/null", NULL);
        exit(7);                    /* 여기에 도달할 수는 없음 */
    }

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid != 0) {            /* 셋째 자식 */
        sleep(8);
        exit(0);                    /* 정상 종료 */
    }

    sleep(6);                       /* 넷째 자식 */
    kill(getpid(), SIGKILL);        /* 신호에 의해 종료, 코어 덤프는 없음 */            
    exit(6);                        /* 여기에 도달할 수는 없음 */
}