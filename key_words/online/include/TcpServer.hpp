#ifndef __WD_TCPSERVER_H__
#define __WD_TCPSERVER_H__

#include "Acceptor.hpp"
#include "EventLoop.hpp"
#include "TcpConnection.hpp"

namespace wd
{

class TcpServer
{
public:
    TcpServer(const string &ip, unsigned short port);
    void start();
    void stop();

    void setConnectionCallback(TcpConnectionCallback &&cb);
    void setMessageCallback(TcpConnectionCallback &&cb);
    void setCloseCallback(TcpConnectionCallback &&cb);

private:
    Acceptor _acceptor;
    EventLoop _loop;
};

}//end of namespace wd

#endif
