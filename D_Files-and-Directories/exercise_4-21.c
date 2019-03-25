#include "apue.h"
#include <fcntl.h>

int
main(int argc, char *argv[])
{
    int                 i, fd;
    struct stat         statbuf;
    struct timespec     times[2];

    for (i = 1; i < argc; i++) {
        if (stat(argv[i], &statbuf) < 0) {  /* 현재 시간들을 얻는다. */
            err_ret("%s: stat error", argv[i]);
            continue;
        }
        if ((fd = open(argv[i], O_RDWR | O_TRUNC)) < 0) {   /* 파일 절단 */
            err_ret("%s: open errer", argv[i]);
            continue;
        }    
        times[0].tv_sec = statbuf.st_atime;
        times[1].tv_sec = statbuf.st_mtime;
        if (futimens(fd, times) < 0)        /* 시간들을 재설정한다. */
            err_ret("%s: futimens error", argv[i]);
        close(fd);
    }
    /* 최종 수정 시간과 최종 접근 시간은 변하지 않고 상태 변경 시간만 실행된 시간으로 변경 */
    exit(0);
}