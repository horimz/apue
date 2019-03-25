#include    <setjmp.h>
#include    <signal.h>
#include    <unistd.h>

static jmp_buf  env_alrm;

static void
sig_alrm(int signo)
{
    longjmp(env_alrm, 1);
}

unsigned int
sleep2(unsigned int seconds)
{
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        return(seconds);
    if (setjmp(env_alrm) == 0) {
        alarm(seconds);     /* 타이머를 시작한다 */
        pause();            /* 다응번에 신호가 잡히면 프로세스가 깨어난다 */
    }
    return(alarm(0));       /* 타이머를 끄고, 남은 시간을 돌려준다 */
}