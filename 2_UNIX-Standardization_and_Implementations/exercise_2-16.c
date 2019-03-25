#include "apue.h"
#include <errno.h>
#include <limits.h>

#ifdef  PATH_MAX
static long pathmax = PATH_MAX;
#else
static long pathmax = 0;
#endif

static long posix_version = 0;
static long xsi_version = 0;

/* PATH_MAX가 불확정 한계이면 이것이 적합하다는 보장은 없다. */
#define PATH_MAX_GUESS  1024

char *
path_alloc(size_t *sizep)   /* 널이 아닌 경우 할당된 크기로 돌려준다. */
{
    char    *ptr;
    size_t  size;

    if (posix_version == 0)
        posix_version = sysconf(_SC_VERSION);

    if (xsi_version == 0)
        xsi_version = sysconf(_SC_XOPEN_VERSION);

    if (pathmax == 0) {      /* 첫 시도 */
        errno = 0;
        if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0) {
            if (errno == 0)
                pathmax = PATH_MAX_GUESS;   /* 불확정인 경우 */
            else
                err_sys("pathconf error for _PC_PATH_MAX");
        } else {
            pathmax++;      /* 루트에 상대적이므로 1증가 */
        }
    }
    /*
     * POSIX.1-2001 이전에는 PATH_MAX에 종료 널 바이트가 포함된다는
     * 보장이 없었다. XPG3도 마찬가지이다.
     */
    if ((posix_version < 200112L) && (xsi_version < 4))
        size = pathmax + 1;
    if ((ptr = malloc(size)) == NULL)
        err_sys("malloc error for pathname");
    
    if (sizep != NULL)
        *sizep = size;
    return(ptr);
}