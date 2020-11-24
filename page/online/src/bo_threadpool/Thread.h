#ifndef _THREAD_H_
#define _THREAD_H_

#include "NonCopyable.h"
#include <pthread.h>
#include <functional>
#include <string>

namespace Cur_thread_name
{
    extern __thread const char * name;
}

class Thread
:public NonCopyable
{
public:
    using ThreadCallBack=std::function<void()>;
    Thread(ThreadCallBack &&,const std::string & threadName=std::string());
    virtual ~Thread();

    void start();
    void join();
    pthread_t getThreadId() const;
    std::string getThreadName() const;
private:
    static void * threadFunc(void *); //子线程函数

private:
    pthread_t _pthid;
    bool _isRunning;
    ThreadCallBack _cb;
    std::string _threadName;
};

#endif

