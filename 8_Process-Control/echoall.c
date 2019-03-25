#include "apue.h"

int
main(int argc, char *argv[])
{
    int             i;
    char            **ptr;
    extern char     **environ;

    for (i = 0; i < argc; i++)          /* 명령줄 인수들을 모두 출력 */
        printf("argv[%d]: %s\n", i, argv[i]);

    for (ptr = environ; *ptr != 0; ptr++)   /* 환경 문자열들도 모두 출력 */
        printf("%s\n", *ptr);

    exit(0);
}