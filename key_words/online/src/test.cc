#include "../include/TcpServer.hpp"
#include "../include/Thread.hpp"
#include "../include/Threadpool.hpp"
#include "../include/Dict.hpp"
#include "../include/MyTask.hpp"
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

wd::Threadpool * gThreadpool = nullptr;

void onConnection(const wd::TcpConnectionPtr &conn)
{
    cout << ">> " << conn->toString() << " has connected!" << endl;
    /* conn->send("welcome to server!"); */
}

void onMessage(const wd::TcpConnectionPtr &conn)
{
    string msg = conn->receive();
    msg.pop_back(); //去掉末尾换行符
    cout << ">>> receive:" << msg << endl;
    
    //回显操作
    /* conn->send(msg); */
    //此时如果业务处理的时间较长，就不适合在IO线程直接处理
    //应该将受到的数据封装成一个任务，交给线程池处理
    MyTask task(msg, conn, Mydict::getInstance());
    gThreadpool->addTask(std::bind(&MyTask::execute, task));
}

void onClose(const wd::TcpConnectionPtr &conn)
{
    cout << "> " << conn->toString() << " has closed!" << endl;
}

void test()
{
    wd::Threadpool threadpool(4, 10);
	threadpool.start();
	gThreadpool = &threadpool;

    wd::TcpServer server("10.211.55.7", 9999);

    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.setCloseCallback(onClose);
    server.start();
}

int main()
{
    test();
    
    return 0;
}

