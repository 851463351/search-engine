#include "../include/TaskQueue.hpp"

namespace wd
{

TaskQueue::TaskQueue(size_t queSize)
: _queSize(queSize)
, _que()
, _mutex()
, _notEmpty(_mutex)
, _notFull(_mutex)
, _flag(true)
{}

bool TaskQueue::empty() const
{
    return _que.size() == 0;
}

bool TaskQueue::full() const
{
    return _que.size() == _queSize;
}

//该方法运行在生产者线程
void TaskQueue::push(const ElemType &elem)
{
    //1.对互斥锁进行保护，防止发生死锁
    MutexLockGuard autoLock(_mutex);
    //_mutex.lock();
    while(full())
    {
        _notFull.wait(); //2.使用while防止虚假唤醒
    }

    _que.push(elem);
    _notEmpty.notify();

    //_mutex.unlock();
}

//该方法运行在消费者线程
ElemType TaskQueue::pop()
{
    MutexLockGuard autoLock(_mutex);
    while(_flag && empty()) //增加判断条件_flag，防止阻塞
    {
        _notEmpty.wait();
    }

    if(_flag)
    {
        ElemType elem = _que.front();
        _que.pop();

        _notFull.notify();

        return elem;
    }
    else
    {
        return nullptr;
    }
}

void TaskQueue::wakeup()
{
    _flag = false;
    _notEmpty.notifyAll();
}

}//end of namespace wd
