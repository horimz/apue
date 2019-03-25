#include    <sys/wait.h>
#include    <errno.h>
#include    <unistd.h>

int
system(const char *cmdstring)   /* 신호 처리가 없는 버전 */
{
    pid_t   pid;
    int     status;

    if (cmdstring == NULL)
        return(1);      /* UNIX에서는 항상 명령 처리기가 있음 */
    
    if ((pid = fork()) < 0) {
        status = -1;    /* 아마도 최대 프로세스 개수 초과 */
    } else if (pid == 0) {      /* 자식 */
        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
        _exit(127);     /* execl 오류 */
    } else {
        while (waitpid(pid, &status, 0) < 0) {
            if (errno != EINTR) {
                status = -1;    /* waitpid()가 EINTR 이외의 오류를 반환했음 */
                break;
            }
        }
    }
    return(status);
}