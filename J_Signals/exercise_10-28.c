#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

int
system(const char *cmdstring)   /* 신호들을 적절히 처리하는 버전 */
{
    pid_t               pid;
    int                 status;
    struct sigaction    ignore, saveintr, savequit;
    sigset_t            chldmask, savemask;
    
    if (cmdstring == NULL)
        return(1);      /* 유닉스 시스템에는 항상 명령 처리기가 존재한다 */
    
    ignore.sa_handler = SIG_IGN;
    sigemptyset(&ignore.sa_mask);
    ignore.sa_flags = 0;
    if (sigaction(SIGINT, &ignore, &saveintr) < 0)
        return(-1);
    if (sigaction(SIGQUIT, &ignore, &savequit) < 0)
        return(-1);
    sigemptyset(&chldmask);     /* 이제 SIGCHLD를 차단 */
    sigaddset(&chldmask, SIGCHLD);
    if (sigprocmask(SIG_BLOCK, &chldmask, &savemask) < 0)
        return(-1);

    if ((pid = fork()) < 0) {
        status = -1;        /* 아마도 최대 프로세스 개수 초과일 듯 */
    } else if (pid == 0) {  /* 자식 */
        /* 이전 신호 동작들과 신호 마스크를 복원한다 */
        sigaction(SIGINT, &saveintr, NULL);
        sigaction(SIGQUIT, &savequit, NULL);
        sigprocmask(SIG_SETMASK, &savemask, NULL);

        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
        _exit(127);     /* exec 오류 */
    } else {                /* 부모 */
        while (waitpid(pid, &status, 0) < 0)
            if (errno != EINTR) {
                status = -1;    /* waitpid()의 EINTR 이외의 오류 */
                break;
            }
    }

    /* 이전 신호 동작들과 신호 마스크를 복원한다 */
    if (sigaction(SIGINT, &saveintr, NULL) < 0)
        return(-1);
    if (sigaction(SIGQUIT, &savequit, NULL) < 0)
        return(-1);
    if (sigprocmask(SIG_SETMASK, &savemask, NULL) < 0)
        return(-1);

    return(status);
}