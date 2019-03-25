#include "apue.h"

#define BUFFSIZE    1024

static void
sig_tstp(int signo)     /* SIGTSTP를 위한 신호 처리부 */
{
    sigset_t    mask;

    /* ... 커서를 왼쪽 하단 모퉁이로 옮기고 tty 모드를 재설정한다 ... */
    
    signal(SIGTSTP, SIG_DFL);   /* 처분 방식을 기본 동작으로 재설정하고 */
    
    kill(getpid(), SIGTSTP);    /* 프로세스 자신에게 신호를 보낸다 */

    /* kill이 반환될 때까지는 실행이 재개되지 않는다 */

    /*
     * SIGTSTP의 차단을 해제한다(이 신호를 처리하는 동안에는
     * 어차피 차단되므로)
     */
    sigemptyset(&mask);
    sigaddset(&mask, SIGTSTP);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);

    signal(SIGTSTP, sig_tstp);  /* 신호 처리부를 다시 등록한다 */

    /* ... tty 모드를 재설정하고 화면을 다시 그린다 ... */
}

int
main(void)
{
    int     n;
    char    buf[BUFFSIZE];

    /*
     * 작업 제어 셸에서 실행된 경우에만 SIGTSTP를 잡는다.
     */
    if (signal(SIGTSTP, SIG_IGN) == SIG_DFL)
        signal(SIGTSTP, sig_tstp);

    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
        if (write(STDOUT_FILENO, buf, n) != n) 
            err_sys("write error");

    if (n < 0)
        err_sys("read error");
    
    exit(0);
}