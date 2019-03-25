#include <pwd.h>
#include <stddef.h>
#include <string.h>

struct passwd *
getpwnam(const char *name)
{
    struct passwd   *ptr;

    setpwent();
    while ((ptr = getpwent()) != NULL)
        if (strcmp(name, ptr->pw_name) == 0)
            break;      /* 부합하는 항목을 발견 */
    endpwent();
    return(ptr);        /* 부합하는 항목을 발견하지 못했으면 ptr는 NULL임 */
}