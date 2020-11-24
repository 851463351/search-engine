#ifndef __WD_THREAD_HPP__
#define __WD_THREAD_HPP__

#include "Noncopyable.hpp"
#include <pthread.h>
#include <string>
#include <functional>

namespace wd
{

namespace current_thread
{

extern __thread const char *name;

}//end of namespace current_thread

class Thread
: Noncopyable
{
public:
    using ThreadCallback = std::function<void()>;

    Thread(ThreadCallback &&cb, const std::string &name = std::string());   //默认为空字符串
    ~Thread();

    void start();
    void join();

    pthread_t getThreadId() const { return _pthid;  }

    std::string getThreadName() const { return _name;   }

private:
    static void* threadFunc(void *arg);

private:
    pthread_t _pthid;
    bool _isRunning;
    ThreadCallback _cb;
    std::string _name;
};

}//end of namespace wd

#endif
