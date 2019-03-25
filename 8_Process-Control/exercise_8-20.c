#include "apue.h"
#include <sys/wait.h>

int
main(void)
{
    pid_t   pid;
    
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {          /* 자식 */
        if (execl("/Users/HoRim/Documents/Hongik_Univ/2018_2/OPEN_SOURCE_SOFTWARE/unix_exercise/chap07/testinterp", 
                  "testinterp", "myarg1", "MY ARG2", (char *)0) < 0)
            err_sys("execl error");
    }
    if (waitpid(pid, NULL, 0) < 0)  /* 부모 */
        err_sys("waitpid error");
    exit(0);
}