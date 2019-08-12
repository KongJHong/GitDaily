/*
 * @Descripttion: 线程池
 * @version: 
 * @Author: KongJHong
 * @Date: 2019-08-12 15:32:48
 * @LastEditors: KongJHong
 * @LastEditTime: 2019-08-12 15:34:33
 * https://blog.csdn.net/qq_36359022/article/details/78796784
 */


#include "ThreadPool.h"
#include <pthread.h>
#include <cstring>
#include <unistd.h>
#include <csignal>



#define DEFAULT_TIME 1      //默认休眠时间
#define DEFAULT_THREAD_NUM  4 //默认线程增加数

void errmsg(const char* msg);


void *threadpool_thread(void* threadpool);
void *admin_thread(void *threadpool);
int is_thread_alive(pthread_t tid);
int threadpool_free(threadpool_t *pool);

/*任务*/
typedef struct{
    void *(*function)(void*);
    void *arg;
}threadpool_task_t;

/*线程池管理类*/
struct threadpool_t{
    pthread_mutex_t lock;           //锁住整个结构体
    pthread_mutex_t thread_counter; //用于使用忙线程数时的锁
    pthread_cond_t  queue_not_full; //条件变量，任务队列不满
    pthread_cond_t  queue_not_empty;//任务队列不为空

    pthread_t       *threads;       //存放线程的tid,实际上就是管理线程的数组
    pthread_t       admin_tid;       //管理者线程tid
    threadpool_task_t *task_queue;  //任务队列

    /*线程池信息*/
    int min_thr_num;                //线程池中最小线程数
    int max_thr_num;                //线程池中最大线程数目
    int live_thr_num;               //线程池中存活的线程数
    int busy_thr_num;               //忙线程，正在工作的线程
    int wait_exit_thr_num;          //需要销毁的线程数

    /*任务队列信息*/
    int queue_front;                //队头
    int queue_rear;                 //队尾
    int queue_size;

    /*存在的任务数*/
    int queue_max_size;             //队列能容纳的最大任务数

    /*线程池状态*/
    int shutdown;                   //true为关闭

};

threadpool_t*
threadpool_create(int min_thr_num,int max_thr_num,int queue_max_size)
{
    int i;
    threadpool_t *pool = nullptr;

    do
    {
        /*线程池管理空间开辟*/
        if((pool = (threadpool_t*)malloc(sizeof(threadpool_t))) == nullptr)
        {
            printf("malloc threadpool false;\n");
            break;
        }

        /*信息初始化*/
        pool->min_thr_num = min_thr_num;
        pool->max_thr_num = max_thr_num;
        pool->busy_thr_num = 0;
        pool->live_thr_num = min_thr_num;
        pool->wait_exit_thr_num = 0;
        pool->queue_front = 0;
        pool->queue_rear = 0;
        pool->queue_size = 0;
        pool->queue_max_size = queue_max_size;
        pool->shutdown = false;

        /*根据最大线程数，给工作线程数组开辟空间，并清0*/
        pool->threads = (pthread_t*)malloc(sizeof(pthread_t) * max_thr_num);
        if(pool->threads == nullptr)
        {
            errmsg("malloc threads false;");
            break;
        }
        memset(pool->threads,0,sizeof(pthread_t) * max_thr_num);

        /*队列开空间*/
        pool->task_queue =
                (threadpool_task_t*)malloc(sizeof(threadpool_task_t) * queue_max_size);
        if(pool->task_queue == nullptr)
        {
            errmsg("malloc threads false;");
            break;
        }
        memset(pool->task_queue,0,sizeof(threadpool_task_t)*queue_max_size);

        /*初始化互斥锁和条件变量*/
        if(pthread_mutex_init(&(pool->lock),NULL) != 0 ||
            pthread_mutex_init(&(pool->thread_counter),NULL) != 0 ||
            pthread_cond_init(&(pool->queue_not_empty),NULL) != 0 ||
            pthread_cond_init(&(pool->queue_not_full),NULL) != 0
        )
        {
            errmsg("init lock or cond false;");
            break;
        }

        /*启动min_thr_num个工作线程*/
        for(i = 0;i < min_thr_num;i++)
        {
            /*pool指向当前线程池*/
            pthread_create(&(pool->threads[i]),NULL,threadpool_thread,(void*)pool);
            printf("start thread 0x%x...\n",(UINT)pool->threads[i]);
        }

        /*管理者线程*/
        pthread_create(&(pool->admin_tid),NULL,admin_thread,(void*)pool);

        return pool;
    }while(0);

    /*释放pool的空间*/
    threadpool_free(pool);
    return pool;
}


/**
 * 管理线程,用于管理线程池的线程增删
 * @param pool 传入线程池管理指针
 */
void *
admin_thread(void *threadpool)
{
    int i;
    threadpool_t *pool = (threadpool_t*)threadpool;
    while(!pool->shutdown)
    {
        printf("admin --------------------\n");
        sleep(DEFAULT_TIME);                            //休眠一段时间再管理
        pthread_mutex_lock(&(pool->lock));              //加锁
        int queue_size = pool->queue_size;              //任务数
        int live_thr_num = pool->live_thr_num;          //存活的线程数
        pthread_mutex_unlock(&(pool->lock));            //解锁

        pthread_mutex_lock(&(pool->thread_counter));
        int busy_thr_num = pool->busy_thr_num;          //忙碌线程
        pthread_mutex_unlock(&(pool->thread_counter));

        printf("admin busy live -%d--%d-\n",busy_thr_num,live_thr_num);
        /*创建新线程，实际任务数量大于 最小正在等待的任务数量十倍，且存活线程数小于最大线程数*/
        if(queue_size >= (live_thr_num - busy_thr_num) * 10 && live_thr_num <= pool->max_thr_num)
        {
            printf("admin add -------------\n");
            pthread_mutex_lock(&(pool->lock));
            int add = 0;

            //一次增加DEFAULT_THREAD_NUM个线程
            for(i = 0;i < pool->max_thr_num && add < DEFAULT_THREAD_NUM
                        &&pool->live_thr_num < pool->max_thr_num;i++)
            {
                if(pool->threads[i] == 0 || !is_thread_alive(pool->threads[i]))
                {
                    pthread_create(&(pool->threads[i]),NULL,threadpool_thread,(void*)pool);
                    add++;
                    pool->live_thr_num++;
                    printf("new thread --------------------------\n");
                }
            }
            pthread_mutex_unlock(&(pool->lock));
        }

        /*销毁多余的线程，忙线程x2 都小于 存活县城，并且存活的大于最小线程数*/
        if((busy_thr_num*2) < live_thr_num && live_thr_num > pool->min_thr_num)
        {
            /*一次销毁DEFAULT_THREAD_NUM个线程*/
            for(i=0;i < DEFAULT_THREAD_NUM;i++)
            {
                //通知正在处于空闲的线程，自杀
                pthread_cond_signal(&(pool->queue_not_empty));
                printf("admin clear--\n");
            }
        }
    }
    return nullptr;
}

/*线程是否存活*/
int is_thread_alive(pthread_t tid)
{
    int kill_rc = pthread_kill(tid,0);
    return kill_rc != ESRCH;
}


/**
 * 工作线程
 * @param threadpool ：传入线程池管理指针
 * @return
 */
void *
threadpool_thread(void* threadpool)
{
    threadpool_t *pool = (threadpool_t*)threadpool;
    threadpool_task_t task;

    while(true)
    {
        pthread_mutex_lock(&(pool->lock));

        /*无任务阻塞在“任务队列不为空”上，有任务则跳出*/
        while((pool->queue_size == 0) && (!pool->shutdown))
        {
            printf("thread 0x%x is waiting\n",(UINT)pthread_self());
            pthread_cond_wait(&(pool->queue_not_empty),&(pool->lock));

            //判断是否需要清楚线程，自杀功能呢个
            if(pool->wait_exit_thr_num > 0)
            {
                pool->wait_exit_thr_num--;
                //判断线程池中的线程数是否大于最小线程数，是则结束当前线程
                if(pool->live_thr_num > pool->min_thr_num)
                {
                    printf("thread 0x%x is exiting\n",(UINT)pthread_self());
                    pool->live_thr_num--;
                    pthread_mutex_unlock(&(pool->lock));
                    pthread_exit(NULL);//结束线程
                }
            }
        }

        //线程池开关状态
        if(pool->shutdown)  //关闭线程池
        {
            pthread_mutex_unlock(&(pool->lock));
            printf("thread 0x%x is exiting\n",(UINT)pthread_self());
            pthread_exit(NULL); //线程自己结束自己
        }

        //否则该线程可以取出任务
        task.function = pool->task_queue[pool->queue_front].function;
        task.arg = pool->task_queue[pool->queue_front].arg;

        pool->queue_front = (pool->queue_front + 1) % pool->queue_max_size;
        pool->queue_size--;

        //通知可以添加新任务
        pthread_cond_broadcast(&(pool->queue_not_full));

        //释放线程锁
        pthread_mutex_unlock(&(pool->lock));

        //执行刚才取出的任务
        printf("thread 0x%x start working\n",(UINT)pthread_self());
        pthread_mutex_lock(&(pool->thread_counter));
        pool->busy_thr_num++;
        pthread_mutex_unlock(&(pool->thread_counter));


        (*(task.function))(task.arg);

        //任务结束处理
        printf("thread 0x%x end working",(UINT)pthread_self());
        pthread_mutex_lock(&(pool->thread_counter));
        pool->busy_thr_num--;
        pthread_mutex_unlock(&(pool->thread_counter));
    }

    pthread_exit(NULL);
}

int threadpool_add_task(threadpool_t *pool,void *(*function)(void*),void *args)
{
    pthread_mutex_lock(&(pool->lock));

    /*如果队列满了，调用wait阻塞*/
    while((pool->queue_size == pool->queue_max_size) && (!pool->shutdown))
    {
        pthread_cond_wait(&(pool->queue_not_full),&(pool->lock));
    }

    //如果线程处于关闭状态
    if(pool->shutdown)
    {
        pthread_mutex_unlock(&(pool->lock));
        return -1;
    }

    /*清空工作线程的毁掉函数的采纳书arg*/
    if (pool->task_queue[pool->queue_rear].arg != nullptr)
    {
        free(pool->task_queue[pool->queue_rear].arg);
        pool->task_queue[pool->queue_rear].arg = nullptr;
    }

    /*添加任务到任务队列*/
    pool->task_queue[pool->queue_rear].function = function;
    pool->task_queue[pool->queue_rear].arg = args;
    pool->queue_rear = (pool->queue_rear + 1) % pool->queue_max_size;
    pool->queue_size++;

    /*添加完任务后，队列就不为空了，唤醒线程池中的一个线程*/
    pthread_cond_signal(&(pool->queue_not_empty));
    pthread_mutex_unlock(&(pool->lock));

    return 0;
}


/**
 * 释放线程池
 * @param pool 线程池管理指针
 * @return -1表示错误，0表示正确
 */
int threadpool_free(threadpool_t *pool)
{
    if(pool == nullptr)return -1;

    if(pool->task_queue)free(pool->task_queue);

    if(pool->threads)
    {
        free(pool->threads);
        pthread_mutex_trylock(&(pool->lock));
        pthread_mutex_unlock(&(pool->lock));
        pthread_mutex_destroy(&(pool->lock));

        pthread_mutex_trylock(&(pool->thread_counter));
        pthread_mutex_unlock(&(pool->thread_counter));
        pthread_mutex_destroy(&(pool->thread_counter));

        pthread_cond_destroy(&(pool->queue_not_empty));
        pthread_cond_destroy(&(pool->queue_not_full));
    }

    free(pool);
    pool = nullptr;
    return 0;
}

int threadpool_destroy(threadpool_t *pool)
{
    int i;
    if(!pool)return -1;

    pool->shutdown = true;

    /*销毁管理者线程*/
    pthread_join(pool->admin_tid,NULL);

    //通知所有线程去自杀
    for(i = 0;i < pool->live_thr_num;i++)
    {
        pthread_cond_broadcast(&(pool->queue_not_empty));
    }

    //等待线程结束，先是pthread_exit,然后等待结束
    for(i = 0;i < pool->live_thr_num;i++)
    {
        pthread_join(pool->threads[i],NULL);
    }

    threadpool_free(pool);
    return 0;
}


void errmsg(const char* msg)
{
    printf("%d error:%s \n",__LINE__,msg);
}
