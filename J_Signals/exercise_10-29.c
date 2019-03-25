#include "apue.h"

static void
sig_alrm(int signo)
{
    /* 아무 것도 하지 않고 반환, 그러면 sigsuspend()가 깨어난다 */
}

unsigned int
sleep(unsigned int seconds)
{
    struct sigaction    newact, oldact;
    sigset_t            newmask, oldmask, suspmask;
    unsigned int        unslept;

    /* 신호 처분 방식을 저장하고 새 신호 처리부를 설정한다 */
    newact.sa_handler = sig_alrm;
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;
    sigaction(SIGALRM, &newact, &oldact);

    /* SIGALRM을 차단하고 현재 신호 마스크를 저장해 둔다 */
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGALRM);
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);

    alarm(seconds);
    suspmask = oldmask;

    /* SIGALRM이 차단되지 않도록 한다 */
    sigdelset(&suspmask, SIGALRM);

    /* 임의의 신호가 잡히길 기다린다 */
    sigsuspend(&suspmask);

    /* 신호가 잡혔다. 이제 SIGALRM은 차단된 상태이다. */

    unslept = alarm(0);

    /* 이전 신호 처분 방식을 복원한다. */
    sigaction(SIGALRM, &oldact, NULL);

    /* 신호 마스크를 복원한다(SIGALRM의 차단이 해제됨) */
    sigprocmask(SIG_SETMASK, *oldmask, NULL);
    return(unslept);
}