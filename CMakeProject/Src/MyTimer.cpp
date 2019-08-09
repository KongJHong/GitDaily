/*
 * @Descripttion: 
 * @version: 
 * @Author: KongJHong
 * @Date: 2019-08-09 20:07:11
 * @LastEditors: KongJHong
 * @LastEditTime: 2019-08-09 21:39:36
 */
#include "MyTimer.h"

#include <sys/time.h>

/******************************Timer***********************************/

MyTimer::MyTimer(TimerManager &manager)
    :manager_(manager)
    ,m_nHeapIndex(-1)
{
}

MyTimer::~MyTimer()
{
    stop();
}



void MyTimer::stop()
{
    if(m_nHeapIndex != -1)
    {
        manager_.remove_timer(this);
        m_nHeapIndex = -1;
    }
}

void MyTimer::tick(ULLONG now)
{
    if(timerType_ == TimerType::CIRCLE)
    {
        m_nExpires = m_nInterval + now;
        manager_.add_timer(this);
    }
    else
    {
        m_nHeapIndex = -1;
    }

    m_timerFunc();
}

/******************************TimerManager***********************************/

void TimerManager::add_timer(MyTimer *timer)
{
    //插到数组最后一个位置上，上浮
    timer->m_nHeapIndex = heap_.size();
    HeapEntry entry = {timer->m_nExpires,timer};
    heap_.push_back(entry);
    up_heap(heap_.size() - 1);
}


void TimerManager::remove_timer(MyTimer *timer)
{

    //remove_timer既可能发生在定时器响应，也可能发生在外部停止
    //所以，index并不总是在堆顶，既0
    
    //头元素用数组末尾元素替换，然后下沉
    size_t index = timer->m_nHeapIndex;   

    if(!heap_.empty() && index < heap_.size())
    {
        if(index == heap_.size() - 1)//只有一个定时器
        {   
            heap_.pop_back();
        }
        else            //多个定时器
        {
            swap_heap(index,heap_.size() - 1);
            heap_.pop_back();

            size_t parent = (index-1)/2;        
            if(index > 0 && heap_[index].time < heap_[parent].time)
                up_heap(index);
            else
                down_heap(index);
        }   
    }
}

void TimerManager::detect_timers()
{
    ULLONG now = get_current_millisecs();
    
    while(!heap_.empty() && heap_[0].time <= now)
    {
        MyTimer *timer = heap_[0].timer;
        remove_timer(timer);
        timer->tick(now);
    }
}


void TimerManager::up_heap(size_t index)
{
    //下至上，和父节点比较。如果小于父节点，则上浮
    size_t parent = (index-1)/2;
    while(index > 0 && heap_[index].time < heap_[parent].time)
    {
        swap_heap(index,parent);
        index = parent;
        parent = (index-1)/2;
    }
}


void TimerManager::down_heap(size_t index)
{
    //从下到上，算出左右子节点，和最小的交换
    size_t child = index*2+1;
    int heap_len = heap_.size();
    while(child < heap_len)
    {
        if(child + 1 < heap_len && heap_[child].time > heap_[child+1].time)
            child++;
        if(heap_[child].time < heap_[index].time)
            break;
        swap_heap(index,child);
        index = child;
        child = index * 2 + 1;
    }
}

void TimerManager::swap_heap(size_t index1,size_t index2)
{
    HeapEntry tmp = heap_[index1];
    heap_[index1] = heap_[index2];
    heap_[index2] = tmp;
    heap_[index1].timer->m_nHeapIndex = index1;
    heap_[index2].timer->m_nHeapIndex = index2;
}

typename TimerManager::ULLONG TimerManager::get_current_millisecs()
{
    timeval tv;
    ::gettimeofday(&tv,0);
    ULLONG ret = tv.tv_sec;
    
    return ret * 1000 + tv.tv_usec / 1000;
}
