#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void    
abort(void)     /* POSIX 스타일 abort() 함수 */
{
    sigset_t            mask;
    struct sigaction    action;

    /* 호출자는 SIGABRT를 무시할 수 없다. 만일 무시로 설정되어
       있으면 기본 동작으로 재설정한다. */
    sigaction(SIGABRT, NULL, &action);
    if (action.sa_handler == SIG_IGN) {
        action.sa_handler == SIG_DFL;
        sigaction(SIGABRT, &action, NULL);
    }
    if (action.sa_handler == SIG_DFL)
        fflush(NULL);           /* 열린 모든 표준 입출력 스트림을 방출 */
    
    /* 호출자는 SIGABRT를 차단할 수 없다. 명시적으로 차단을 해제한다. */
    sigfillset(&mask);
    sigdelset(&mask, SIGABRT); 
    sigprocmask(SIG_SETMASK, &mask, NULL);
    kill(getpid(), SIGABRT);    /* 신호를 보낸다 */

    /* 여기에 도달했다면 SIGABRT가 잡히고 해당 신호 처리부가 반환된 것이다 */
    fflush(NULL);               /* 열린 모든 표준 입출력 스트림을 방출 */
    action.sa_handler = SIG_DFL;
    sigaction(SIGABRT, &action, NULL);  /* 기본 동작으로 재설정 */
    sigprocmask(SIG_SETMASK, &mask, NULL);  /* 혹시 모르므로... */
    kill(getpid(), SIGABRT);            /* 확실하게 종료 */
    exit(1);    /* 여기에 도달해서는 안된다... */
}