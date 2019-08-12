/*
 * @Descripttion: 
 * @version: 
 * @Author: KongJHong
 * @Date: 2019-08-12 15:32:41
 * @LastEditors: KongJHong
 * @LastEditTime: 2019-08-12 15:33:02
 */
//
// Created by hong on 8/11/19.
//

#ifndef THREADPOOL_THREADPOOL_H
#define THREADPOOL_THREADPOOL_H
#include <iostream>

typedef unsigned int UINT;

struct threadpool_t;

/**
 * 向线程池中添加一个任务
 * @param pool          目标线程池
 * @param function      回调函数
 * @param args          回调函数的参数
 * @return              -1失败，0成功
 */
int threadpool_add_task(threadpool_t *pool,void *(*function)(void*),void *args);

/**
 * 创建线程池
 * @param min_thr_num       最小线程数
 * @param max_thr_num       最大线程数
 * @param queue_max_size    最大任务处理
 * @return
 */
threadpool_t* threadpool_create(int min_thr_num,int max_thr_num,int queue_max_size);


/**
 * 销毁线程池
 * @param pool      指定要销毁的线程池
 * @return      -1失败，0成功
 */
int threadpool_destroy(threadpool_t *pool);




#endif //THREADPOOL_THREADPOOL_H
