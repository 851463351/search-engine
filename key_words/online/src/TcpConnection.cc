#include "../include/TcpConnection.hpp"
#include "../include/EventLoop.hpp"
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sstream>

namespace wd
{

TcpConnection::TcpConnection(int fd, EventLoop *loop)
: _loop(loop)
, _sock(fd)
, _socketIo(fd)
, _localAddr(getLocalAddr(fd))
, _peerAddr(getPeerAddr(fd))
, _isShutdownWrite(false)
{}

TcpConnection::~TcpConnection()
{
    if(!_isShutdownWrite)
        shutdown();
}

string TcpConnection::receive()
{
    char buf[65536] = {0};
    int ret = _socketIo.readline(buf, sizeof(buf));
    printf("recv len = %d\n", ret);
    return string(buf);
}

void TcpConnection::send(const Message &msg)
{
    int ret = _socketIo.writen(&msg, 8 + msg._len);
    printf("msg len = %d\n", sizeof(msg));
    printf("msg._len = %d\n", msg._len);
    printf("send len = %d\n", ret);
}

void TcpConnection::sendInLoop(const Message &msg)
{
    if(_loop)
        _loop->runInLoop(std::bind(&TcpConnection::send, this, msg));   //将TcpConnection类中的发送函数封装注册给IO线程
}

string TcpConnection::toString() const
{
    std::ostringstream oss;
    oss << _localAddr.ip() << ":" << _localAddr.port() << " --> " << _peerAddr.ip() << ":" << _peerAddr.port();
    return oss.str();
}

void TcpConnection::shutdown()
{
    if(!_isShutdownWrite)
    {
        _isShutdownWrite = true;
        _sock.shutdownWrite();
    }
}

InetAddress TcpConnection::getLocalAddr(int fd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    if(getsockname(_sock.fd(), (struct sockaddr *)&addr, &len) == -1)
        perror("getsockname");
    return InetAddress(addr);
}

InetAddress TcpConnection::getPeerAddr(int fd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    if(getpeername(_sock.fd(), (struct sockaddr *)&addr, &len) == -1)
        perror("getpeername");
    return InetAddress(addr);
}

void TcpConnection::setConnectionCallback(const TcpConnectionCallback &cb)
{   _onConnection = cb; }

void TcpConnection::setMessageCallback(const TcpConnectionCallback &cb)
{   _onMessage = cb; }

void TcpConnection::setCloseCallback(const TcpConnectionCallback &cb)
{   _onClose = cb;  }

void TcpConnection::handleConnectionCallback()
{
    if(_onConnection)
        _onConnection(shared_from_this());
}

void TcpConnection::handleMessageCallback()
{
    if(_onMessage)
        _onMessage(shared_from_this());
}

void TcpConnection::handleCloseCallback()
{
    if(_onClose)
        _onClose(shared_from_this());
}

bool TcpConnection::isConnectionClose()
{
    int nready = -1;
    char buf[128] = {0};
    do
    {
        nready = ::recv(_sock.fd(), buf, sizeof(buf), MSG_PEEK);
    } while(nready == -1 && errno == EINTR);
    return nready == 0;
}

}//end of namespace wd
