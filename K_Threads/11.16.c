// 장벽을 이용하는 방법을 보여주는 프로그램

#include "apue.h"
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>
#include <stdint.h>

#define PTHREAD_BARRIER_SERIAL_THREAD   1

typedef struct pthread_barrier {
  pthread_mutex_t         mutex;
  pthread_cond_t          cond;
  volatile uint32_t       flag;
  size_t                  count;
  size_t                  num;
} pthread_barrier_t;

int pthread_barrier_init(pthread_barrier_t *bar, int attr, int num)
{
  int ret = 0;
  if ((ret = pthread_mutex_init(&(bar->mutex), 0))) return ret;
  if ((ret = pthread_cond_init(&(bar->cond), 0))) return ret;
  bar->flag = 0;
  bar->count = 0;
  bar->num = num;
  return 0;
}

int pthread_barrier_wait(pthread_barrier_t *bar)
{
  int ret = 0;
  uint32_t flag = 0;

  if ((ret = pthread_mutex_lock(&(bar->mutex)))) return ret;

  flag = bar->flag;
  bar->count++;

  if (bar->count == bar->num) {
    bar->count = 0;
    bar->flag = 1 - bar->flag;
    if ((ret = pthread_cond_broadcast(&(bar->cond)))) return ret;
    if ((ret = pthread_mutex_unlock(&(bar->mutex)))) return ret;
    return PTHREAD_BARRIER_SERIAL_THREAD;
  }

  while (1) {
    if (bar->flag == flag) {
      ret = pthread_cond_wait(&(bar->cond), &(bar->mutex));
      if (ret) return ret;
      } else { break; }
    }

  if ((ret = pthread_mutex_unlock(&(bar->mutex)))) return ret;
  return 0;
}

#define NTHR 8                  /* 스레드 개수 */
#define NUMNUM 8000000L         /* 정렬할 수치 개수 */
#define TNUM (NUMNUM/NTHR)      /* 스레드당 정렬할 수치 개수 */

long nums[NUMNUM];
long snums[NUMNUM];

pthread_barrier_t b;

#ifdef SOLARIS
#define heapsort qsort
#else
extern int heapsort(void *, size_t, size_t, int (*)(const void *, const void *));
#endif

/* 두 긴 정수를 비교한다(heapsort 함수의 보조 함수) */
int complong(const void *arg1, const void *arg2) {
    long l1 = *(long *)arg1;
    long l2 = *(long *)arg2;

    if (l1 == l2)       /* 길이가 같다 */
        return 0;
    else if (l1 < l2)   /* l2가 더 길다 */
        return -1;
    else                /* l1가 더 길다 */
        return 1;
}

/* 일단의 수들의 일부를 정렬하는 일꾼 스레드 */
void *
thr_fn(void *arg) {
    long idx = (long)arg;

    heapsort(&nums[idx], TNUM, sizeof(long), complong);
    pthread_barrier_wait(&b);

    /*
     * ... 나머지 필요한 작업을 수행 ...
     */
    return((void *)0);
}

/* 부분적인 정렬 결과들을 병합한다 */
void
merge() {
    long idx[NTHR];
    long i, minidx, sidx, num;

    for (i = 0; i < NTHR; i++)
        idx[i] = i * TNUM;
    for (sidx = 0; sidx < NUMNUM; sidx++) {
        num = LONG_MAX;
        for (i = 0; i < NTHR; i++) {
            if ((idx[i] < (i+1)*TNUM) && (nums[idx[i]] < num)) {
                num = nums[idx[i]];
                minidx = i;
            }
        }
        snums[sidx] = nums[idx[minidx]];
        idx[minidx]++;
    }
}

int main() {
    unsigned long i;
    struct timeval start, end;
    long long startusec, endusec;
    double elapsed;
    int err;
    pthread_t tid;

    /* 정렬할 일단의 수치들을 생성한다 */
    srandom(1);
    for (i = 0; i < NUMNUM; i++)
        nums[i] = random();

    /* 스레드 여덟 개를 생성해서 수치들을 정렬한다 */
    gettimeofday(&start, NULL);
    pthread_barrier_init(&b, NULL, NTHR+1);
    for (i = 0; i < NTHR; i++) {
        err = pthread_create(&tid, NULL, thr_fn, (void *)(i * TNUM));
        if (err != 0)
            err_exit(err, "can't create thread");
    }
    pthread_barrier_wait(&b);
    merge();
    gettimeofday(&end, NULL);

    /* 정렬된 목록을 출력한다 */
    startusec = start.tv_sec * 1000000 + start.tv_usec;
    endusec = end.tv_sec * 1000000 + end.tv_usec;
    elapsed = (double)(endusec = startusec) / 1000000.0;
    printf("sort took %.4f seconds\n", elapsed);
    for (i = 0; i < NUMNUM; i++)
        printf("%ld\n", snums[i]);
    exit(0);
}