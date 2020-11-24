#include "../include/Threadpool.hpp"
#include "../include/Thread.hpp"
#include <unistd.h>

namespace wd
{

Threadpool::Threadpool(size_t threadNum, size_t queSize)
: _threadNum(threadNum)
, _queSize(queSize)
, _taskQueue(_queSize)
, _isExit(false)
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
    //创建子线程
    for(size_t i = 0; i != _threadNum; ++i)
    {
        unique_ptr<Thread> up(new Thread(std::bind(&Threadpool::threadFunc, this), std::to_string(i)));
        _threads.push_back(std::move(up));  //使用移动语义
    }

    //子线程开始运行
    for(auto &pthread : _threads)
    {
        pthread->start();
    }
}

void Threadpool::stop()
{
    if(!_isExit)
    {
        //当任务队列中还有任务时，需要再等一等
        while(!_taskQueue.empty())
        {
            ::sleep(1);
        }
        
        //当退出while循环时，意味着任务队列中没有任务了
        _isExit = true;
        _taskQueue.wakeup();    //防止有子线程阻塞在pop中的wait()

        for(auto &pthread : _threads)
        {
            pthread->join();
        }
    }
}

void Threadpool::addTask(Task &&task)
{
    _taskQueue.push(std::move(task));
}

Task Threadpool::getTask()
{
    return _taskQueue.pop();
}

void Threadpool::threadFunc()
{
    while(!_isExit)
    {
        Task task = getTask();
        if(task)
        {
            task();
        }
    }
}

}//end of namespace wd
