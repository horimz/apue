#include "apue.h"
#include <setjmp.h>
#include <time.h>

static void                     sig_usr1(int);
static void                     sig_alrm(int);
static sigjmp_buf               jmpbuf;
static volatile sig_atomic_t    canjump;

int 
main(void)
{
    if (signal(SIGUSR1, sig_usr1) == SIG_ERR)
        err_sys("signal(SIGUSR1) error");
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        err_sys("siganl(ALRM) error");
    
    pr_mask("starting main: ");     /* 10.14 */

    if (sigsetjmp(jmpbuf, 1)) {
        pr_mask("ending main: ");
        exit(0);
    }
    canjump = 1;    /* sigsetjmp()가  성공적으로 호출되었음 */

    for ( ; ; )
        pause();
}

static void
sig_usr1(int signo)
{
    time_t  starttime;

    if (canjump == 0)
        return;     /* 기대하지 않은 신호이므로 무시한다 */
    
    pr_mask("starting sig_usr1: ");
    
    alarm(3);       /* 3초 후 SIGALRM 신호 발생 */
    starttime = time(NULL);

    for ( ; ; )     /* 수면 없이 5초 간 대기 */
        if (time(NULL) > starttime + 5)
            break;
    
    pr_mask("finishing sig_usr1: ");

    canjump = 0;
    siglongjmp(jmpbuf, 1);  /* main으로 분기, 여기로 반환되지는 않음 */
}

static void 
sig_alrm(int signo)
{
    pr_mask("in sig_alrm: ");
}