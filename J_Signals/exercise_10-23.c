#include "apue.h"

volatile sig_atomic_t   quitflag;   /* 신호 처리부에 의해 0이 아닌 값으로 설정됨 */

static void
sig_int(int signo)      /* SIGINT와 SIGQUIT을 하나의 신호 처리부로 잡는다 */
{
    if (signo == SIGINT)
        printf("\ninterrupt\n");
    else if (signo == SIGQUIT)
        quitflag = 1;   /* 주 푸르를 위해 플래그를 설정한다 */
}

int
main(void)
{
    sigset_t    newmask, oldmask, zeromask;

    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal(SIGINT) error");
    if (signal(SIGQUIT, sig_int) == SIG_ERR)
        err_sys("signal(SIGQUIT) error");
    
    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);

    /* 
     * SIGQUIT을 차단하고 현재 신호 마스크를 저장한다. 
     */ 
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        err_sys("SIG_BLOCK error");
    
    while (quitflag == 0)
        sigsuspend(&zeromask);

    /* 
     * SIGQUIT이 잡혔고 이제 차단된 상태이다.
     * ...필요한 작업을 수행한다...
     */
    quitflag = 0;

    /*
     * 신호 마스크를 복원한다(이에 의해 SIGQUIT의 차단이 해제됨).
     */
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("SIG_SETMASK error");

    exit(0);
}