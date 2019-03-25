#include "apue.h"

int     globval = 6;        /* 초기화된 자료 구역의 외부 변수 */

int 
main(void)
{
    int     var;        /* 스택의 자동 변수 */
    pid_t   pid;

    var = 88;
    printf("before vfork\n");   /* 표준 입출력을 방출하지 않음 */
    if ((pid = vfork()) < 0) {
        err_sys("vfork error");
    } else if (pid == 0) {      /* 자식 */
        globval++;              /* 부모의 변수들을 수정 */
        var++;
        _exit(0);
        //exit(0);
    }

    /* 부모의 실행이 여기서부터 재기된다. */
    printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globval, var);
    exit(0);
}