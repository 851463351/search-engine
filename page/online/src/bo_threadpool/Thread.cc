#include "Thread.h"


namespace Cur_thread_name
{
    __thread const char * name="non-name thread "; 
}

struct threadData
{
     std::string _name;
     Thread::ThreadCallBack _cb;

     threadData(const std::string & name,Thread::ThreadCallBack &cb)
         :_name(name)
         ,_cb(cb)
     {}
     void runInThread()
     {
        Cur_thread_name::name=(_name==std::string())? "non-name thread" :_name.c_str();
        if(_cb)
        {
            _cb();
        }
     }
};


Thread::Thread(ThreadCallBack && cb,const std::string & threadName)
    :_pthid(0)
    ,_isRunning(false)
    ,_cb(std::move(cb))
    ,_threadName(threadName) {}

Thread::~Thread()
{
    if(_isRunning)
    {
        pthread_detach(_pthid);
    }
}


void Thread::start()
{
    threadData * p=new threadData(_threadName,_cb);
    pthread_create(&_pthid,nullptr,threadFunc,p);
    _isRunning=true;
}

void Thread::join()
{
    if(_isRunning)
    {
        pthread_join(_pthid,nullptr);
        _isRunning=false;
    }
}

pthread_t Thread::getThreadId() const
{
    return _pthid;
}

std::string Thread::getThreadName() const
{
    return _threadName;
}

void *Thread::threadFunc(void *arg)
{
    threadData *p=static_cast<threadData*>(arg);
    if(p)
    {
        p->runInThread(); 
    }
    return nullptr;
}
