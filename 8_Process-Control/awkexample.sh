#!/usr/bin/awk -f
# 참고: Solaris에서는 awk 대신 nawk를 사용할 것.
BEGIN {
    for (i = 0; i < ARGC; i++)
        printf "ARGV[%d] = %s\n", i, ARGV[i]
    exit
}