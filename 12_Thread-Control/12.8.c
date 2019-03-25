// 재귀적 뮤텍스의 사용법을 보여주는 프로그램 

#include "apue.h"
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

extern int makethread(void *(*)(void *), void *);

struct to_info {
    void (*to_fn)(void *);      /* 함수 */
    void *to_arg;               /* 인수 */
    struct timespec to_wait;    /* 기다릴 시간 */
};

#define SECTONSEC 1000000000

#if !defined(CLOCK_REALTIME) || defined(BSD)
#define clock_nanosleep(ID, FL, REQ, REM) nanosleep((REQ), (REM))
#endif

#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#define USECTONSEC 1000

void
clock_gettime(int id, struct timespec *tsp) {
    struct timeval tv;

    gettimeofday(&tv, NULL);
    tsp->tv_sec = tv.tv_sec;
    tsp->tv_nsec = tv.tv_usec * USECTONSEC;
}
#endif

void *
timeout_helper(void *arg) {
    struct to_info *tip;

    tip = (struct to_info *)arg;
    clock_nanosleep(CLOCK_REALTIME, 0, &tip->to_wait, NULL);
    (*tip->to_fn)(tip->to_arg);
    free(arg);
    return(0);
}

void timeout(const struct timespec *when, void (*func)(void *), void *arg) {
    struct timespec now;
    struct to_info *tip;
    int err;

    clock_gettime(CLOCK_REALTIME, &now);
    if ((when->tv_sec > now.tv_sec) || (when->tv_sec == now.tv_sec && when->tv_nsec > now.tv_nsec)) {
        tip = malloc(sizeof(struct to_info));
        if (tip != NULL) {
            tip->to_fn = func;
            tip->to_arg = arg;
            tip->to_wait.tv_sec = when->tv_sec - now.tv_sec;
            if (when->tv_nsec >= now.tv_nsec) {
                tip->to_wait.tv_nsec = when->tv_nsec - now.tv_nsec;
            } else {
                tip->to_wait.tv_sec--;
                tip->to_wait.tv_nsec = SECTONSEC - now.tv_nsec + when->tv_nsec;
            }
            err = makethread(timeout_helper, (void *)tip);
            if (err == 0)
                return;
            else
                free(tip);
        }
    }

    /* 여기에 도달했다면 무언가 실패한 것이다 */
    (*func)(arg);
}

pthread_mutexattr_t attr;
pthread_mutex_t mutex;

void
retry(void *arg) {
    pthread_mutex_lock(&mutex);

    /* ... 재시도 단계들을 수행 ... */
    
    pthread_mutex_unlock(&mutex);
}

int
main(void) {
    int err, condition, arg;
    struct timespec when;

    if ((err = pthread_mutexattr_init(&attr)) != 0)
        err_exit(err, "pthread_mutexattr_init failed");
    if ((err = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE)) != 0)
        err_exit(err, "can't set resursive type");
    if ((err = pthread_mutex_init(&mutex, &attr)) != 0)
        err_exit(err, "can't create recursive mutex");
    
    /* ... 처리를 계속 진행 ... */

    pthread_mutex_lock(&mutex);

    /* 조건 점검과 timeout 호출을 원자적으로 수행하기 위해 자물쇠로 보호 중인 조건의 상태를 점검한다 */
    if (condition) {
        /* 재시도할 절대 시간을 계산한다 */
        clock_gettime(CLOCK_REALTIME, &when);
        when.tv_sec += 10;
        timeout(&when, retry, (void *)((unsigned long)arg));
    }
    pthread_mutex_unlock(&mutex);

    /* ... 처리를 계속 진행 ... */
    
    exit(0);
}