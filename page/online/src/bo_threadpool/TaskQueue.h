#ifndef _TASKQUEUE_H_
#define _TASKQUEUE_H_

#include "Condition.h"
#include "Task.h"
#include <queue>

using std::queue;


class TaskQueue
{
public:
    using ElemType=Task;
    TaskQueue(size_t queSize);

    bool empty() const;
    bool full() const;
    void push(ElemType);
    ElemType pop();
    void wakeup();
private:
    size_t _queSize;
    queue<ElemType> _que;
    MutexLock _mutex;
    Condition _notEmply;
    Condition _notFull;
    bool _flag; //标记线程退出时，解锁 
};
#endif
