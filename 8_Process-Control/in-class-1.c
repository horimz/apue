#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    pid_t pid, ppid;

    printf("My PID is: %d\n\n", getpid());
    printf("Par PID is: %d\n\n", getppid());

    return 0;
}