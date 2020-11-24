#ifndef __WD_TASKQUEUE_HPP__
#define __WD_TASKQUEUE_HPP__

#include "MutexLock.hpp"
#include "Condition.hpp"
#include "Task.hpp"
#include <queue>

using std::queue;

namespace wd
{

using ElemType = Task;

class TaskQueue
{
public:
    TaskQueue(size_t queSize);

    bool empty() const;
    bool full() const;
    void push(const ElemType &elem);
    ElemType pop();

    void wakeup();

private:
    size_t _queSize;
    queue<ElemType> _que;
    MutexLock _mutex;
    Condition _notEmpty;
    Condition _notFull;
    bool _flag; //标志位，防止阻塞在pop()中的wait()
};

}//end of namespace wd

#endif
