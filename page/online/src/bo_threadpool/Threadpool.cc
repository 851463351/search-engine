#include "Threadpool.h"
#include "Task.h"

#include <unistd.h>

Threadpool::Threadpool(size_t threadNum,size_t queSize)
    :_threadNum(threadNum)
    ,_queSize(queSize)
    ,_taskQueue(_queSize)
    ,_isExit(false)
{
    _threads.reserve(_threadNum);
}

Threadpool::~Threadpool()
{
    if(!_isExit)
    {
        stop();
    }

}

void Threadpool::start()
{
    //创建线程对象
    for(size_t idx=0;idx<_threadNum;++idx)
    {
        unique_ptr<Thread> up(new Thread(std::bind(&Threadpool::threadFunc,this),std::to_string(idx)));
        _threads.push_back(std::move(up));
    }

    //启动线程
    for(auto & pthread :_threads)
    {
        pthread->start();
    }
}

void Threadpool::stop()
{
    if(!_isExit)
    {
        //当任务队列中还有任务时需要等一等
        while(!_taskQueue.empty()){
            ::sleep(1);
        }
        _isExit=true;
        _taskQueue.wakeup();
        for(auto &pthread :_threads)
        {
            
            pthread->join();
        }
    }
}

void Threadpool::addTask(Task ptask)
{
    _taskQueue.push(ptask);
}
Task Threadpool::getTask()
{
    return _taskQueue.pop();
}

void Threadpool::threadFunc()
{
    while(!_isExit)
    {
        Task pTask=getTask();
        if(pTask)
        {
            pTask();
        }
    }
}
