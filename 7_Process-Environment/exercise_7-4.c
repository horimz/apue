#include "apue.h"

int
main(int argc, char *argv[])
{
    int     i;

    /*
    for (i = 0; i < argc; i++)      // 명령줄 인수들을 모두 출력 
        printf("argv[%d]: %s\n", i, argv[i]);
    */

    for (i = 0; argv[i] != NULL; i++)
        printf("argv[%d]: %s\n", i, argv[i]);
        
    exit(0);
}