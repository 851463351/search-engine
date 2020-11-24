#include "../include/Thread.hpp"
#include <stdio.h>

namespace wd
{

namespace current_thread
{

__thread const char *name = "wd thread";

}//end of namespace current_thread

using ThreadCallback = Thread::ThreadCallback;

struct ThreadData   //定义管理线程名的类，方便拓展
{
    std::string _name;
    ThreadCallback _cb;

    ThreadData(const std::string &name, const ThreadCallback &cb)
    : _name(name)
    , _cb(cb)
    {}

    void runInThread()
    {
        //在回调函数执行之前进行threadName的赋值
        current_thread::name = (_name == std::string()) ? "wd thread" : _name.c_str();

        if(_cb)
            _cb();

        //...可拓展
    }
};

Thread::Thread(ThreadCallback &&cb, const std::string &name)
: _pthid(0)
, _isRunning(false)
, _cb(std::move(cb))
, _name(name)
{}

Thread::~Thread()
{
    if(_isRunning)
    pthread_detach(_pthid);
}

void Thread::start()
{
    ThreadData* pdata = new ThreadData(_name, _cb);

    if(pthread_create(&_pthid, nullptr, threadFunc, pdata))
    {
        perror("pthread_create");
        return;
    }
    _isRunning = true;
}

void Thread::join()
{
    if(_isRunning)
    {
        pthread_join(_pthid, nullptr);
        _isRunning = false;
    }
}

void* Thread::threadFunc(void *arg)
{
    ThreadData *p = static_cast<ThreadData *>(arg);
    if(p)
        p->runInThread();
    
    delete p;
    return nullptr;
}

}//end of namespace wd
