#ifndef __WD_MUTEX_HPP__
#define __WD_MUTEX_HPP__

#include "Noncopyable.hpp"
#include <pthread.h>

namespace wd
{

class MutexLock
: Noncopyable
{
public:
    MutexLock();
    ~MutexLock();

    void lock();
    void unlock();

    pthread_mutex_t* getMutexLockPtr()
    {
        return &_mutex;
    }

private:
    pthread_mutex_t _mutex;
};

class MutexLockGuard
{
public:
    MutexLockGuard(MutexLock &mutex)
    : _mutex(mutex)
    {
        _mutex.lock();
    }

    ~MutexLockGuard()
    {
        _mutex.unlock();
    }

private:
    MutexLock &_mutex;
};

}//end of namespace wd

#endif
