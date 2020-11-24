#ifndef _ECHOSERVER_H_
#define _ECHOSERVER_H_

#include "TcpServer.h"
#include "../bo_threadpool/Threadpool.h"


class EchoServer
{
public:
    EchoServer(size_t threadNum,size_t queSize,const string & ip,unsigned short port)
        :_threadpool(threadNum,queSize)
        ,_tcpServer(ip,port) {}

    void start()
    {
        _threadpool.start();
        _tcpServer.start();
    }

    void stop()
    {
        _threadpool.stop();
        _tcpServer.stop();
    }

    Threadpool * getThreadpool()   {return &_threadpool;}

    void setConnectionCallBack(TcpConnectionCallBack && cb) {_tcpServer.setConnectionCallBack(std::move(cb));}
    void setMessageCallBack(TcpConnectionCallBack &&cb) {_tcpServer.setMessageCallBack(std::move(cb));}
    void setCloseCallBack(TcpConnectionCallBack &&cb) {_tcpServer.setCloseCallBack(std::move(cb));}

private:
    Threadpool _threadpool;
    TcpServer _tcpServer;

};

#endif
