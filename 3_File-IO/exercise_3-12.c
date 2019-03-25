#include "apue.h"
#include <fcntl.h>

void
set_fl(int fd, int flags)   /* flags 인수는 켜고자 하는 파일 상태 플래그들 */
{
    int     val;

    if ((val = fcntl(fd, F_GETFL, 0)) < 0)
        err_sys("fcntl F_GETFL error");

    val |= flags;           /* 플래그들을 켠다 */
    
    if (fcntl(fd, F_SETFL, val) < 0)
        err_sys("fcntl F_SETFL error");
}