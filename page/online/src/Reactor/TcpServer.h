#ifndef _TCPSERVER_H_
#define _TCPSERVER_H_

#include "Acceptor.h"
#include "EventLoop.h"

class TcpServer
{
public:
    TcpServer(const string &ip,unsigned short port); 

    void start();
    void stop();

    void setConnectionCallBack(TcpConnectionCallBack && cb);
    void setMessageCallBack(TcpConnectionCallBack &&cb);
    void setCloseCallBack(TcpConnectionCallBack &&cb);

private:
    Acceptor _acceptor;
    EventLoop _loop;

};

#endif
