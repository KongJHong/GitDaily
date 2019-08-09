/*
 * @Descripttion: CPP定时器，仿boost
 * @version: 
 * @Author: KongJHong
 * @Date: 2019-08-09 19:01:35
 * @LastEditors: KongJHong
 * @LastEditTime: 2019-08-09 21:42:37
 * 1.定时器分为只执行一次，循环执行
 * 2.借鉴Boost，利用最小堆实现
 * 3.多线程的业务线程中不包含定时管理器，单独开一个线程用来管理所有定时器
 *   当时间出发时，向业务线程投递定时器消息即可。
 */
// #pragma once

#ifndef MYTIMER_H
#define MYTIMER_H

#include <iostream>
#include <functional>
#include <vector>

class TimerManager;


//定时器类
class MyTimer
{

public:
    typedef unsigned int UINT;
    typedef unsigned long long  ULLONG;
    typedef int INT;
public:

    MyTimer() = default;

    enum class TimerType{ONCE=0,CIRCLE=1};
    MyTimer(TimerManager &manager);   
    ~MyTimer(); 

    /**
     * 启动一个定会器
     * func :到期调用的方法
     * ms   :定时时间，相对时间，单位：毫秒
     * type :定时类型，单次/循环
     */
    void start(std::function<void(void)> func,UINT ms,TimerType type);
    //外部终止一个定时器
    void stop();

private:
    //定时器到时执行
    void tick(ULLONG now);

private:
    friend class TimerManager;
    TimerManager &manager_;

    //调用函数，包括仿函数 https://blog.csdn.net/u010710458/article/details/79734558
    std::function<void(void)> m_timerFunc;

    TimerType timerType_;

    //间隔
    UINT m_nInterval;

    //过期
    ULLONG m_nExpires;

    //在heap中的下标
    INT m_nHeapIndex;
};


//定时器管理类
class TimerManager
{
    
public:
    typedef unsigned long long  ULLONG;
    typedef unsigned int        UINT;
    
public:
    //获取当前毫秒数
    static ULLONG get_current_millisecs();

    //探测执行
    void detect_timers();

private:
    friend class MyTimer;

    //添加一个定时器
    void add_timer(MyTimer *timer);
    //移除一个定时器
    void remove_timer(MyTimer *timer);
    //定时上浮(传入下标)
    void up_heap(size_t index);
    //定时下沉(传入下标)
    void down_heap(size_t index);
    //交换两个timer索引
    void swap_heap(size_t index1,size_t index2);

private:
    struct HeapEntry
    {
        ULLONG time;
        MyTimer *timer;
    };
    std::vector<HeapEntry> heap_;
};



inline void MyTimer::start(std::function<void(void)> func,UINT interval,TimerType type)
{
    m_nInterval = interval;
    m_timerFunc = func;
    m_nExpires = interval + TimerManager::get_current_millisecs();
    manager_.add_timer(this);
    timerType_ = type;
}

#endif