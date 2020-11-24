#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include "TaskQueue.h"
#include "Thread.h"

#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;


class Threadpool
{
    friend class WorkThread;
public:
    Threadpool(size_t threadNum,size_t queSize);
    ~Threadpool();
    void start();
    void stop();
    void addTask(Task);
private:
    Task getTask();
    void threadFunc();
    void wakeup();
private:
    size_t _threadNum;
    size_t _queSize;
    vector<unique_ptr<Thread>> _threads;
    TaskQueue _taskQueue;
    bool _isExit;

};

#endif
