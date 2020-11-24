#ifndef _CONDITION_H_
#define _CONDITION_H_

#include "NonCopyable.h"
#include "MutexLock.h"
#include <pthread.h>


class Condition
:public NonCopyable
{
public:
    Condition(MutexLock &mutex)
        :_mutex(mutex)
    {
        pthread_cond_init(&_cond,NULL);
    }
    ~Condition()
    {
        pthread_cond_destroy(&_cond);
    }
    void wait()
    {
        pthread_cond_wait(&_cond,_mutex.getMutexLockPtr());
    }
    void notify()
    {
        pthread_cond_signal(&_cond);
    }
    void notifyall()
    {
        pthread_cond_broadcast(&_cond);
    }
private:
    MutexLock & _mutex;
    pthread_cond_t _cond;
};


#endif
