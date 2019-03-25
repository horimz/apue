// reader-writer lock의 사용법을 보여주는 프로그램

#include <stdlib.h>
#include <pthread.h>

struct job {
    struct job *j_next;
    struct job *j_prev;
    pthread_t j_id;
    /* ... 기타 필드들 ... */
};

struct queue {
    struct job *q_head;
    struct job *q_tail;
    pthread_rwlock_t q_lock;
};

/* 대기열을 초기화한다 */
int
queue_init(struct queue *qp) {
    int err;

    qp->q_head = NULL;
    qp->q_tail = NULL;
    err = pthread_rwlock_init(&qp->q_lock, NULL);
    if (err != 0)
        return(err);
    /* ... 나머지 초기화 작업을 진행 ... */
    return(0);
}

/* 작업 하나를 대기열의 머리에 삽입한다 */
void
job_insert(struct queue *qp, struct job *jp) {
    pthread_rwlock_wrlock(&qp->q_lock);
    jp->j_next = qp->q_head;
    jp->j_prev = NULL;
    if (qp->q_head != NULL)
        qp->q_head->j_prev = jp;
    else
        qp->q_tail = jp;    /* 목록이 빈 경우 */ 
    qp->q_head = jp;
    pthread_rwlock_unlock(&qp->q_lock);
} 

/* 작업 하나를 대기열의 꼬리에 추가한다 */
void
job_append(struct queue *qp, struct job *jp) {
    pthread_rwlock_wrlock(&qp->q_lock);
    jp->j_next = NULL;
    jp->j_prev = qp->q_tail;
    if (qp->q_tail != NULL)
        qp->q_tail->j_next = jp;
    else
        qp->q_head = jp;    /* 목록이 빈 경우 */
    pthread_rwlock_unlock(&qp->q_lock);
}

/* 주어진 작업을 대기열에서 제거한다 */
void
job_remove(struct queue *qp, struct job *jp) {
    pthread_rwlock_wrlock(&qp->q_lock);
    if (jp == qp->q_head) {
        qp->q_head = jp->j_next;
        if (qp->q_tail == jp)
            qp->q_tail = NULL;
        else
            jp->j_next->j_prev = jp->j_prev;
    } else if (jp == qp->q_tail) {
        qp->q_tail = jp->j_prev;
        jp->j_prev->j_next = jp->j_next;
    } else {
        jp->j_prev->j_next = jp->j_next;
        jp->j_next->j_prev = jp->j_prev;
    }
    pthread_rwlock_unlock(&qp->q_lock);
}

/* 주어진 스레드 ID에 해당하는 작업을 찾는다 */
struct job *
job_find(struct queue *qp, pthread_t id) {
    struct job *jp;
    
    if (pthread_rwlock_rdlock(&qp->q_lock) != 0)
        return(NULL);
    
    for (jp = qp->q_head; jp != NULL; jp = jp->j_next)
        if (pthread_equal(jp->j_id, id))
            break;

    pthread_rwlock_unlock(&qp->q_lock);
    return(jp);
}