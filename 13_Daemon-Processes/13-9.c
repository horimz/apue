#include "apue.h"
#include <fcntl.h>

int set_cloexec(int fd) {
    int     val;

    if ((val = fcntl(fd, F_GETFD, 0)) < 0)
        return(-1);
    
    val |= FD_CLOEXEC;      /* exec-시-닫기 플래그를 활성화 */
    
    return(fcntl(fd, F_SETFD, val));
}