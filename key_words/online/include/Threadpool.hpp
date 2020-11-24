#ifndef __WD_THREADPOOL_HPP__
#define __WD_THREADPOOL_HPP__

#include "TaskQueue.hpp"
#include "Task.hpp"
#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

namespace wd
{

class Thread;

class Threadpool
{
    friend class WorkerThread;
public:
    Threadpool(size_t threadNum, size_t queSize);
    ~Threadpool();

    void start();
    void stop();
    void addTask(Task &&task);  //Task为std::function<void()>，task变量本身为左值，需传入一个右值

private:
    Task getTask();
    void threadFunc();  //要交给农民工兄弟做的事

private:
    size_t _threadNum;
    size_t _queSize;
    vector<unique_ptr<Thread>> _threads;
    TaskQueue _taskQueue;
    bool _isExit;
};

}//end of namespace wd

#endif
