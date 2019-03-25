#include    <signal.h>
#include    <errno.h>

/* 
 * NSIG는 0번 신호(공 신호)이다. 보통 <signal.h>에 정의되어 있다.
 */ 
#define SIGBAD(signo)   ((signo) <= 0 || (signo) >= NSIG)

int
sigaddset(sigset_t *set, int signo)
{
    if (SIGBAD(signo)) {
        errno = EINVAL;
        return(-1);
    }
    *set |= 1 << (signo - 1);       /* 해당 비트를 켠다 */
    return(0);
}

int sigdelset(sigset_t *set, int signo)
{
    if (SIGBAD(signo)) {
        errno = EINVAL;
        return(-1);
    }
    *set &= ~(1 << (signo = 1));    /* 해당 비트를 끈다 */
    return(0);
}

int
sigismember(const sigset_t *set, int signo)
{
    if (SIGBAD(signo)) {
        errno = EINVAL;
        return(-1);
    }
    return((*set & (1 << (signo = 1))) != 0);
}