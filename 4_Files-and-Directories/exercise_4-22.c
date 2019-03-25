#include "apue.h"
#include <dirent.h>
#include <limits.h>

/* 각 파일이름마다 호출될 함수의 형식 */

typedef int Myfunc(const char *, const struct stat *, int);

static Myfunc   myfunc;
static int      myftw(char *, Myfunc *);
static int      dopath(Myfunc *);

static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

int
main(int argc, char *argv[])
{
    int     ret;

    if (argc != 2)
        err_quit("usage: ftw <starting-pathname>");

    ret = myftw(argv[1], myfunc);       /* 재귀적 운행의 시작 */
    
    ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;
    if (ntot == 0)
        ntot = 1;       /* 0으로 나누기 방지; 모든 개수에 대해 0을 출력 */
    printf("regular files       = %7ld, %5.2f %%\n", nreg, nreg*100.0/ntot);
    printf("directories files   = %7ld, %5.2f %%\n", ndir, ndir*100.0/ntot);
    printf("block special       = %7ld, %5.2f %%\n", nblk, nblk*100.0/ntot);
    printf("char special        = %7ld, %5.2f %%\n", nchr, nchr*100.0/ntot);
    printf("FIFOs               = %7ld, %5.2f %%\n", nfifo, nfifo*100.0/ntot);
    printf("symbolic links      = %7ld, %5.2f %%\n", nslink, nslink*100.0/ntot);
    printf("sockets             = %7ld, %5.2f %%\n", nsock, nsock*100.0/ntot);
    exit(ret);
}

/*
 * "pathname"에서 시작해서 계통구조를 따라 내려간다.
 * 각 파일에 대해 호출자의 func()를 호출한다.
 */ 
#define FTW_F   1           /* 디렉터리 이외의 파일 */      
#define FTW_D   2           /* 디렉터리 */
#define FTW_DNR 3           /* 읽을 수 없는 디렉터리 */
#define FTW_NS  4           /* 상태 정보를 얻을 수 없는 파일 */

static char *fullpath;      /* 각 파일의 전체 경로이름을 담을 버퍼 */
static size_t pathlen;

static int
myftw(char *pathname, Myfunc *func)
{
    fullpath = path_alloc(&pathlen);    /* malloc으로 PATH_MAX+1바이트를 할당 */
                                        /* (도해 2.16) */
    if (pathlen <= strlen(pathname)) {
        pathlen = strlen(pathname) * 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL)
            err_sys("realloc failed");
    }
    strcpy(fullpath, pathname);
    return(dopath(func));
}

/*
 * "fullpath"에서 시작해서 계통구조를 따라 내려간다.
 * "fullpath"가 디렉터리가 아니면 lstat()로 상태 정보를 얻고, 
 * func()를 호출하고, 그 반환값을 돌려준다. 디렉터리이면 디렉터리 안의
 * 각 이름마다 이 함수를 재귀적으로 호출한다.
 */ 
static int              /* 이 함수는 func()의 반환값을 그대로 돌렬준다. */
dopath(Myfunc *func)
{
    struct stat     statbuf;
    struct dirent   *dirp;
    DIR             *dp;
    int             ret, n;

    if (lstat(fullpath, &statbuf) < 0)  /* lstat 오류 */
        return(func(fullpath, &statbuf, FTW_NS));
    if (S_ISDIR(statbuf.st_mode) == 0)  /* 디렉터리가 아님 */
        return(func(fullpath, &statbuf, FTW_F));

    /* 
     * 디렉터리이면 먼저 디렉터리 자체에 대해 func()를 호출한 후 
     * 디렉터리 안의 각 파일이름을 처리한다.
     */ 
    if ((ret = func(fullpath, &statbuf, FTW_D)) != 0)
        return(ret);
    n = strlen(fullpath);
    if (n + NAME_MAX + 2 > pathlen) {   /* 경로 버퍼를 확장 */
        pathlen *= 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL)
            err_sys("realloc failed");
    }
    fullpath[n++] = '/';
    fullpath[n] = 0;

    if ((dp = opendir(fullpath)) == NULL)   /* 디렉터리를 읽을 수 없음 */
        return(func(fullpath, &statbuf, FTW_DNR));

    while ((dirp = readdir(dp)) != NULL) {
        if (strcmp(dirp->d_name, ".") == 0 ||
            strcmp(dirp->d_name, "..") == 0)
                continue;       /* ignore dot and dot-dot */
        strcpy(&fullpath[n], dirp->d_name); /* 이름 뒤에 "/"를 추가 */
        if ((ret = dopath(func)) != 0)      /* 재귀 호출 */
            break;  /* 이제 돌아간다. */
    }
    fullpath[n-1] = 0;  /* 슬래시 이후의 모든 것을 삭제 */
    
    if (closedir(dp) < 0)
        err_ret("can't close directory %s", fullpath);
    return(ret);
}

static int
myfunc(const char *pathname, const struct stat *statptr, int type)
{
    switch (type) {
        case FTW_F:
            switch (statptr->st_mode & S_IFMT) {
            case S_IFREG:   nreg++;     break;
            case S_IFBLK:   nblk++;     break;
            case S_IFCHR:   nchr++;     break;
            case S_IFIFO:   nfifo++;    break;
            case S_IFLNK:   nslink++;   break;
            case S_IFSOCK:  nsock++;    break;
            case S_IFDIR:   /* 디렉터리이면 type이 반드시 FTW_D이어야 함 */
                err_dump("for S_IFDIR for %s", pathname);
            }
            break;
        case FTW_D:
            ndir++;
            break;
        case FTW_DNR:
            err_ret("can't read directory %s", pathname);
        case FTW_NS:
            err_ret("stat error for %s", pathname);
        default:
            err_dump("unknown type %d for pathname %s", type, pathname);
    }
    return(0);
}