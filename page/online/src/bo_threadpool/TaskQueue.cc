#include "TaskQueue.h"
#include"Task.h"

TaskQueue::TaskQueue(size_t queSize)
    :_queSize(queSize)
    ,_que()
    ,_mutex()
    ,_notEmply(_mutex)
    ,_notFull(_mutex)
    ,_flag(true){}

bool TaskQueue::empty() const
{
    return _que.size()==0;
}

bool TaskQueue::full() const
{
    return _que.size()==_queSize;
}

void TaskQueue::push(ElemType val)
{
    //对互斥锁进行保护，防止发生死锁
    MutexLockGuard autoLock(_mutex);

    while(full()) //使用while防止虚假唤醒引发问题
    {
        _notFull.wait();
    }

    _que.push(val);
    _notEmply.notify(); //有概率会出现一个notify会唤醒多个线程的情况
}

TaskQueue::ElemType TaskQueue::pop()
{
    MutexLockGuard autoLock(_mutex);

    while(_flag&&empty())  //当线程退出时，flag被置为false，并唤醒所有线程，处于wait上的线程解除阻塞
    {
        _notEmply.wait();
    }

    if(_flag)  //线程退出时，任务队列为空，flag被置为false，直接返回空指针
    {
        TaskQueue::ElemType number=_que.front();
        _que.pop();
    
         _notFull.notify();

        return number;
    }
    else
    {
        return nullptr;
    }
}

void TaskQueue::wakeup()
{
    _flag=false;
    _notEmply.notifyall();
}
