#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    pid_t pid;
    int i;
    
    pid = fork();
    
    if (pid > 0) {      /* Parent */
        for (i = 0; i < 1000; i++)
            printf("\t\t\tPARENT %d\n", i);
    } else {            /* Child */
        for (i= 0; i < 1000; i++)
            printf("CHILD %d\n", i);
    }
    return 0;
}