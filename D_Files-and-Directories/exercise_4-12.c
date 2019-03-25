#include "apue.h"

int
main(void)
{
    struct stat     statbuf;
    /* 그룹-ID-설정 비트를 켜고 그룹 실행 비트를 끈다 */
    if (stat("foo", &statbuf) < 0)
        err_sys("stat error for foo");
    if (chmod("foo", (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0)
        err_sys("chmod error for foo");
    /* 절대 모드를 "rw-r--r--"로 설정한다 */
    if (chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)
        err_sys("chmod error for bar");
    exit(0);
}