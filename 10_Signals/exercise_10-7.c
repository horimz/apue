#include <signal.h>
#include <unistd.h>

static void
sig_alrm(int singo)
{
    /* 아무것도 하지 않고 그대로 반환한다(그러면 pause가 깨어난다) */
}

unsigned int
sleep1(unsigned int seconds)
{
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        return(seconds);
    alarm(seconds);         /* 타이머를 시작한다 */
    pause();                /* 다음번에 신호가 잡히면 프로세스가 깨어난다 */
    return(alarm(0));       /* 타이머를 끄고, 남은 시간을 돌려준다 */
}