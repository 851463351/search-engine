#include "TcpServer.h"

TcpServer::TcpServer(const string &ip,unsigned short port)
    :_acceptor(ip,port)
    ,_loop(_acceptor)
{}

void TcpServer::start()
{
    _acceptor.ready();
    _loop.loop();
}

void TcpServer::stop()
{
    _loop.unloop();
}

void TcpServer::setConnectionCallBack(TcpConnectionCallBack && cb)
{
    _loop.setConnectionCallBack(std::move(cb));
}

void TcpServer::setMessageCallBack(TcpConnectionCallBack && cb)
{
    _loop.setMessageCallBack(std::move(cb));
}

void TcpServer::setCloseCallBack(TcpConnectionCallBack && cb)
{
    _loop.setCloseCallBack(std::move(cb));
}

