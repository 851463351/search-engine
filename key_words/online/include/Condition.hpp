#ifndef __WD_CONDITION_HPP__
#define __WD_CONDItioN_HPP__

#include "Noncopyable.hpp"
#include <pthread.h>

namespace wd
{

class MutexLock;

class Condition
: Noncopyable
{
public:
    Condition(MutexLock &);
    ~Condition();

    void wait();
    void notify();
    void notifyAll();

private:
    pthread_cond_t _cond;
    MutexLock &_mutex;
};

}//end of namespace wd

#endif
