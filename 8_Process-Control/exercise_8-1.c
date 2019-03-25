#include "apue.h"

int     globval = 6;        /* 초기화된 자료 구역의 외부 변수 */
char    buf[] = "a write to stdout\n";

int
main(void)
{
    int     var;    /* 스택의 자동 변수 */
    pid_t   pid;

    var = 88;
    if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1)
        err_sys("write error");
    printf("before fork\n");    /* stdout을 방출하지 않음 */
    
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {      /* 자식 */
        globval++;              /* 부모의 변수들을 수정 */
        var++;
    } else {
        sleep(2);               /* 부모 */
    }
    printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globval, var);
    /* 출력해보면 자식의 변수들은 바뀌고 부모의 복사본은 바뀌지 않음 */
    exit(0);
}