#include "apue.h"
#include <pthread.h>
#include <syslog.h>

sigset_t    mask;

extern int already_running(void);

void reread(void) {
    /* ... */
}

void *thr_fn(void *arg) {
    int err, signo;

    for (;;) {
        err = sigwait(&mask, &signo);               // Wait for a signal (Return 0 on success)
        if (err != 0) {
            syslog(LOG_ERR, "sigwait failed");
            exit(1);
        }

        switch (signo) {
            case SIGHUP:
                syslog(LOG_INFO, "Re-reading configuration file");
                reread();
                break;
                
            case SIGTERM:
                syslog(LOG_INFO, "got SIGTERM; exiting");
                exit(0);

            default:
                syslog(LOG_INFO, "unexpected signal %d\n", signo);
        }
    }
    return(0);
}

int main(int argc, char *argv[]) {
    int                 err;
    pthread_t           tid;
    char                *cmd;
    struct sigaction    sa;

    if ((cmd = strrchr(argv[0], '/')) == NULL)      // cmd에 마지막 '/' 포함해서 이후의 문자들이 저장
        cmd = argv[0];
    else
        cmd++;

    daemonize(cmd);

    if (already_running()) {
        syslog(LOG_ERR, "daemon already running");
        exit(1);
    }

    /* SIGHUP의 기본 동작을 복원하고 다른 모든 신호를 차단 */
    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0)
        err_quit("%s: can't restore SIGHUP default");
    sigfillset(&mask);
    if ((err = ptrhead_sigmask(SIG_BLOCK, &mask, NULL)) != 0)       // ?
        err_exit(err, "SIG_BLOCK error");
    
    /* SIGHUP과 SIGTERM을 처리할 스레드를 생성 */
    err = pthread_create(&tid, NULL, thr_fn, 0);                    // ?
    if (err != 0)
        err_exit(err, "can't create thread");
    
    /* 
     * 데몬의 실제 작업을 진행 
     */
    /* ... */
    exit(0);
}