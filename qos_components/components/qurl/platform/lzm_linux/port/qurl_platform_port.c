/** @file         qurl_platform_port.c
 *  @brief        Brief description: Used to adapt the platform-side interfaces required by qurl.
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-09-08 10:56:48
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#include <pthread.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <time.h>
#include <mqueue.h>
#include <errno.h>
#include "qurl_platform_port.h"

// OS
struct port_pthread_arg_s
{
    qurl_thread_cb func;
    void          *arg;
};
typedef struct port_pthread_arg_s port_pthread_arg_t;

typedef void *(*pthread_func)(void *);

void *port_pthread_func(void *arg)
{
    port_pthread_arg_t *pthread_arg = (port_pthread_arg_t *)arg;
    qurl_thread_cb      func = pthread_arg->func;
    void               *func_arg = pthread_arg->arg;

    free(pthread_arg);
    func(func_arg);

    pthread_exit(NULL);  //Thread exit
}

qurl_ecode_t qurl_platform_thread_create(qurl_thread_t *thread_ptr, qurl_thread_cb func, unsigned char priority, unsigned int stack_size, void *arg)
{
    int                 ret = 0;
    pthread_t           thread_id;
    pthread_attr_t      attr;
    port_pthread_arg_t *port_p_a_ptr = malloc(sizeof(port_pthread_arg_t));

    if (port_p_a_ptr == NULL)
    {
        return QURL_ECODE_NO_MEMORY;
    }

    port_p_a_ptr->func = func;
    port_p_a_ptr->arg = arg;

    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, stack_size);

    //Set thread priority
    struct sched_param param;
    param.sched_priority = priority;
    pthread_attr_setschedparam(&attr, &param);

    //Create thread
    ret = pthread_create(&thread_id, &attr, port_pthread_func, (void *)port_p_a_ptr);
    if (ret != 0)
    {
        pthread_attr_destroy(&attr);
        free(port_p_a_ptr);
        printf("Failed to create thread: %d\n", ret);
        return QURL_ECODE_THREAD_CREATE_FAILED;
    }

    pthread_attr_destroy(&attr);

    *thread_ptr = (qurl_thread_t)thread_id;

    return QURL_OK;
}
qurl_ecode_t qurl_platform_thread_delete(qurl_thread_t thread_ptr)
{
    qurl_ecode_t ret = QURL_OK;

    pthread_cancel((pthread_t)thread_ptr);
    pthread_join((pthread_t)thread_ptr, NULL);

    return ret;
}

//Sleep ms
qurl_ecode_t qurl_platform_sleep_ms(unsigned int ms)
{
    usleep(ms * 1000);  //Call the usleep() function for delay
    return QURL_OK;
}

//Lock Creation
qurl_ecode_t qurl_platform_mutex_create(qurl_mutex_t *lock_ptr)
{
    qurl_ecode_t        ret = 0;
    pthread_mutexattr_t attr = {0};

    if (lock_ptr == NULL)
    {
        return QURL_ECODE_PARAM_INVALID;
    }

    *lock_ptr = malloc(sizeof(pthread_mutex_t));
    if (*lock_ptr == NULL)
    {
        return QURL_ECODE_NO_MEMORY;  //Failed to create mutex
    }

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

    ret = pthread_mutex_init(*lock_ptr, &attr);

    pthread_mutexattr_destroy(&attr);

    if (ret != 0)
    {
        free(*lock_ptr);
        return QURL_ECODE_LOCK_CREATE_FAILD;  //Failed to initialize mutex
    }

    return ret == 0 ? QURL_OK : QURL_ECODE_LOCK_CREATE_FAILD;
}

//Delete lock
qurl_ecode_t qurl_platform_mutex_delete(qurl_mutex_t lock)
{
    qurl_ecode_t ret = 0;

    if (lock == NULL)
    {
        ret = QURL_ECODE_PARAM_INVALID;
    }
    pthread_mutex_destroy(lock);

    return ret == 0 ? QURL_OK : QURL_ECODE_LOCK_DELETE_FAILD;
}

//Lock
qurl_ecode_t qurl_platform_mutex_lock(qurl_mutex_t lock, int timeout_ms)
{
    qurl_ecode_t ret = 0;

    if (lock == NULL)
    {
        ret = QURL_ECODE_PARAM_INVALID;
    }

    if (timeout_ms == QURL_WAIT_FOREVER)
    {
        ret = pthread_mutex_lock(lock);
    }
    else if (timeout_ms > 0)
    {
        struct timeval now;
        gettimeofday(&now, NULL);

        struct timespec ts;
        ts.tv_sec = now.tv_sec + timeout_ms / 1000;
        ts.tv_nsec = (now.tv_usec + (timeout_ms % 1000) * 1000) * 1000;

        ret = pthread_mutex_timedlock(lock, &ts);
    }
    else
    {
        ret = pthread_mutex_trylock(lock);
    }

    switch (ret)
    {
        case 0: {
            ret = QURL_OK;
        }
        break;
        case ETIMEDOUT: {
            ret = QURL_ECODE_LOCK_TIMEOUT;
        }
        break;
        default:
            ret = QURL_ECODE_LOCK_LOCK_FAILD;
            break;
    }

    return ret;
}

//Release lock
qurl_ecode_t qurl_platform_mutex_unlock(qurl_mutex_t lock)
{
    qurl_ecode_t ret = 0;

    if (lock == NULL)
    {
        return QURL_ECODE_PARAM_INVALID;
    }
    ret = pthread_mutex_unlock(lock);

    return ret == 0 ? QURL_OK : QURL_ECODE_LOCK_UNLOCK_FAILD;
}

typedef struct
{
    mqd_t           mq;
    pthread_mutex_t mtx;
} qurl_platform_queue_t;
//Event Creation
qurl_ecode_t qurl_platform_event_create(qurl_queue_t *queue_ptr, int cnt)
{
    struct mq_attr attr = {0};
    attr.mq_flags = 0;  //Blocking operation
    attr.mq_maxmsg = cnt;
    attr.mq_msgsize = sizeof(qurl_event_t);

    //Create a message queue
    mq_unlink("/lzm_queue");
    mqd_t mq = mq_open("/lzm_queue", O_CREAT | O_RDWR, 0666, &attr);
    if (mq == (mqd_t)-1)
    {
        return QURL_ECODE_EVENT_FAILD;
    }

    //Store the message queue and mutex in a structure
    qurl_platform_queue_t *q = (qurl_platform_queue_t *)malloc(sizeof(qurl_platform_queue_t));
    q->mq = mq;

    //Initialize mutex
    if (pthread_mutex_init(&q->mtx, NULL) != 0)
    {
        free(q);
        return QURL_ECODE_EVENT_FAILD;
    }

    //Save the pointer to the structure in a pointer of type void*
    *queue_ptr = (void *)q;

    printf("[%x][%x]\n", q, q->mq);
    return QURL_OK;
}

qurl_ecode_t qurl_platform_event_delete(qurl_queue_t queue)
{
    //Get structure
    qurl_platform_queue_t *q = (qurl_platform_queue_t *)queue;

    //Destroy mutex
    if (pthread_mutex_destroy(&q->mtx) != 0)
    {
        return QURL_ECODE_EVENT_FAILD;
    }

    //Delete message queue
    if (mq_close(q->mq) == -1)
    {
        return QURL_ECODE_EVENT_FAILD;
    }

    if (mq_unlink("/lzm_queue") == -1)
    {
        return QURL_ECODE_EVENT_FAILD;
    }

    //Free memory
    free(queue);

    return QURL_OK;
}
qurl_ecode_t qurl_platform_event_wait(qurl_queue_t queue, qurl_event_t *recv_ptr, int timeout_ms)
{
    qurl_ecode_t ret = QURL_OK;
    //Get structure
    qurl_platform_queue_t *q = (qurl_platform_queue_t *)queue;

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    /*[lzm][note][20231102]Debug log, very strange, if it ends with 99x, mq_timedreceive will report a parameter error.*/
    ts.tv_sec += timeout_ms / 1000;               //Convert milliseconds to seconds
    ts.tv_nsec += (timeout_ms % 1000) * 1000000;  //Convert the remaining milliseconds to nanoseconds
    /*[lzm][note][20231102]Then applying this patch worked, haha.*/
    if (ts.tv_nsec >= 1000000000)
    {                                          //If nanoseconds exceed the legal range
        ts.tv_sec += ts.tv_nsec / 1000000000;  //Convert excess nanoseconds to seconds
        ts.tv_nsec = ts.tv_nsec % 1000000000;  //Modulo operation, keep nanoseconds within the legal range
    }

    //Receive message
    if (mq_timedreceive(q->mq, (char *)recv_ptr, sizeof(qurl_event_t), NULL, &ts) == -1)
    {
        if (errno == ETIMEDOUT)
        {
            ret = QURL_ECODE_EVENT_TIMEOUT;
        }
        else
        {
            // printf("[%x][%x][%x][%d]\n", q, q->mq, recv_ptr, sizeof(qurl_event_t));
            // printf("[%d][%d][%ld][%ld]\n", timeout_ms, errno, ts.tv_sec, ts.tv_nsec);
            ret = QURL_ECODE_EVENT_FAILD;
        }

        return ret;
    }

    return QURL_OK;
}

qurl_ecode_t qurl_platform_event_send(qurl_queue_t queue, const qurl_event_t *sent_ptr, int timeout_ms)
{
    //Get structure
    qurl_ecode_t           ret = 0;
    qurl_platform_queue_t *q = (qurl_platform_queue_t *)queue;

    //Set timeout duration
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += timeout_ms / 1000;
    ts.tv_nsec += (timeout_ms % 1000) * 1000000;
    if (ts.tv_nsec >= 1000000000)
    {
        ts.tv_sec += 1;
        ts.tv_nsec -= 1000000000;
    }

    printf("[%x][%x]\n", q, q->mq);
    //Send message
    if (mq_timedsend(q->mq, (const char *)sent_ptr, sizeof(qurl_event_t), 0, &ts) == -1)
    {
        if (errno == ETIMEDOUT)
        {
            ret = QURL_ECODE_EVENT_TIMEOUT;
        }
        else
        {
            ret = QURL_ECODE_EVENT_FAILD;
        }

        printf("[%x][%x][%d]\n", q, q->mq, errno);
        //Unlock
        pthread_mutex_unlock(&q->mtx);

        return ret;
    }

    return QURL_OK;
}

//Get Thread ID
unsigned long qurl_platform_get_pthread_id(void)
{
    pthread_t tid = pthread_self();
    return (unsigned long)tid;
}

//Get current time
qurl_timeval_t qurl_platform_time_now(void)
{
    qurl_timeval_t timeval = {0};
    struct timeval now = {0};

    gettimeofday(&now, NULL);
    timeval.s = now.tv_sec;
    timeval.ms = now.tv_usec / 1000;

    return timeval;
}

// SOCKET
qurl_ecode_t qurl_platform_socket_nonblock(qurl_socket_t sockfd, int nonblock)
{
    int flags = fcntl(sockfd, F_GETFL, 0);

    return fcntl(sockfd, F_SETFL, nonblock ? flags | O_NONBLOCK : flags & ~O_NONBLOCK) < 0 ? QURL_ECODE_SOCK_CTRL_FAILED : QURL_OK;
}

int qurl_platform_socket_get_errno(qurl_socket_t sockfd)
{
    (void)sockfd;
    return errno;
}

// FS
long qurl_platform_fgetsize(qurl_file_t fd)
{
    struct stat st = {0};

    if (fstat(fd, &st) == 0)
    {
        return st.st_size;
    }
    else
    {
        return 0;
    }
}

int qurl_platform_socket_dns_get(const char *nodename, const char *servname, struct addrinfo *hints, struct addrinfo **ai, int nw_id)
{
#if defined(CONFIG_QOSA_ASR1903_PLATFORM_FUNC) || defined(CONFIG_QOSA_RDA8910_R06_PLATFORM_FUNC) || defined(CONFIG_QOSA_ASR1805_PLATFORM_FUNC)
    return getaddrinfo_with_pcid(nodename, servname, hints, ai, nw_id);
#elif defined(CONFIG_QOSA_QCX216_PLATFORM_FUNC) || defined(CONFIG_QOSA_QCX217_PLATFORM_FUNC) || defined(CONFIG_QOSA_EIGEN718_PLATFORM_FUNC)
    return getaddrinfowithcid(nodename, servname, hints, ai, nw_id);
#else
    return getaddrinfo(nodename, servname, hints, ai);
#endif
}
