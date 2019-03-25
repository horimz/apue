#include "apue.h"
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>

void daemonize(const char *cmd) {
    int                 i, fd0, fd1, fd2;
    pid_t               pid;
    struct rlimit       rl;
    struct sigaction    sa;   

    /* 파일 모드 생성 마스크를 비운다 */
    umask(0);

    /* 가장 큰 파일 서술자 번호를 얻는다 */
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
        err_quit("%s: can't get file limit", cmd);

    /* 새 세션의 리더가 되어서 제어 터미널을 떼어 버린다 */
    if ((pid = fork()) < 0)
        err_quit("%s: can't fork", cmd);
    else if (pid != 0)  /* 부모 */
        exit(0);
    setsid();

    /* 이후의 open 호출 시 제어 터미널이 할당되지 않게 한다 */
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0)
        err_quit("%s: can't ignore SIGHUP", cmd);
    if ((pid = fork()) < 0)
        err_quit("%s: can't fork", cmd);
    else if (pid != 0)  /* 부모 */
        exit(0);

    /* 파일 시스템의 마운트를 해제하지 못하게 되는 일이 생기지 않도록, 현재 작업 디렉터리를 루트로 변경 */
    if (chdir("/") < 0)
        err_quit("%s: can't change directory to /", cmd);

    /* 열린 파일 서술자를 모두 닫는다 */
    if (rl.rlim_max == RLIM_INFINITY)
        rl.rlim_max = 1024;
    for (i = 0; i < rl.rlim_max; i++)
        close(i);

    /* 파일 서술자 0, 1, 2를 /dev/null에 연결 */
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    /* 로그 파일을 초기화 */
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd1, fd1, fd2);
        exit(1);
    }    
}