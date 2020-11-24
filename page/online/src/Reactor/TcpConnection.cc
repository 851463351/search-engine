#include "TcpConnection.h"

#include <sstream>
#include <iostream>

TcpConnection::TcpConnection(int fd,EventLoop* loop)
    :_sock(fd)
    ,_socketIO(fd)
    ,_localAddr(getLocalAddr(fd))
    ,_peerAddr(getPeerAddr(fd))
    ,_isShutdownWrite(false)
    ,_loop(loop)
{}

TcpConnection::~TcpConnection()
{
    if(!_isShutdownWrite)
    {
        shutdown();
    }
}

string TcpConnection::receive()
{
    char buff[65536]={0};
    _socketIO.readline(buff,sizeof(buff));
    return string(buff);
}

void TcpConnection::send(const Message &msg)
{
    /* std::cout<<"4+4+msg._len="<<4+4+msg._len<<std::endl; */
    _socketIO.writen(&msg,4+4+msg._len);
    
}

void TcpConnection::sendInloop(const Message &msg)
{
    if(_loop)
    {
        _loop->runInloop(std::bind(&TcpConnection::send,this,msg));
    }
}

string TcpConnection::toString() const
{
    std::ostringstream oss;
    oss<<_localAddr.ip()<<":"<<_localAddr.port()<<"--->"
        <<_peerAddr.ip()<<":"<<_peerAddr.port();
    
    return oss.str();
}

void TcpConnection::shutdown()
{
    if(!_isShutdownWrite)
    {
        _isShutdownWrite=true;
        _sock.shutdownWrite();
    }
}

void TcpConnection::setConnectionCallBack(const TcpConnectionCallBack & cb)
{
    _onConnection=cb;
}

void TcpConnection::setMesssageCallBack(const TcpConnectionCallBack & cb)
{
    _onMessage=cb;
}

void TcpConnection::setCloseCallBack(const TcpConnectionCallBack & cb)
{
    _onClose=cb;
}

void TcpConnection::handleConnectionCallBack()
{
    if(_onConnection)
    {
        _onConnection(shared_from_this());
    }
}

void TcpConnection::handleMessageCallBack()
{
    if(_onMessage)
    {
        _onMessage(shared_from_this());
    }
}

void TcpConnection::handleCloseCallBack()
{
    if(_onClose)
    {
        _onClose(shared_from_this());
    }
}

InetAdress TcpConnection::getLocalAddr(int fd)
{
    struct sockaddr_in addr;
    socklen_t len=sizeof(struct sockaddr);
    int ret=::getsockname(fd,(struct sockaddr*)&addr,&len);
    if(-1==ret)
    {
        perror("getsockname");
    }
    return InetAdress(addr);
}

InetAdress TcpConnection::getPeerAddr(int fd)
{
    struct sockaddr_in addr;
    socklen_t len=sizeof(struct sockaddr);
    int ret=::getpeername(fd,(struct sockaddr *)&addr,&len);
    if(-1==ret)
    {
        perror("getpeername");
    }
    return InetAdress(addr);
}

bool TcpConnection::isConnectionClose()
{
    int nready=-1;
    char buff[128]={0};
    do{
        nready=::recv(_sock.fd(),buff,sizeof(buff),MSG_PEEK);
    }while(-1==nready && errno==EINTR);

    return nready==0;
}
