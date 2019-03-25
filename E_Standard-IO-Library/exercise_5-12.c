#include "apue.h"

int
main(void)
{
    char    name[L_tmpnam], line[MAXLINE];
    FILE    *fp;

    printf("%s\n", tmpnam(NULL));       /* 첫 임시 이름 */

    tmpnam(name);                       /* 둘째 임시 이름 */
    printf("%s\n", name);

    if ((fp = tmpfile()) == NULL)       /* 임시 파일을 생성 */
        err_sys("tmpfile error");
    fputs("one line of output\n", fp);  /* 임시 파일에 자료를 기록하고 */
    rewind(fp);                         /* 다시 읽어 들인다 */
    if (fgets(line, sizeof(line), fp) == NULL)
        err_sys("fgets error");
    fputs(line, stdout);                /* 기록한 줄을 출력한다 */
    
    exit(0);
}