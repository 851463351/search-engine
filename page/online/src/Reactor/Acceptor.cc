#include "Acceptor.h"

#include <sys/types.h>
#include <sys/socket.h>

Acceptor::Acceptor(unsigned short port)
    :_addr(port)
    ,_listensock()
{}

Acceptor::Acceptor(const string & ip,unsigned short port)
    :_addr(ip,port)
    ,_listensock()
{}

void Acceptor::ready()
{
    setReuseAddr(true);
    setReusePort(true);
    bind();
    listen();
}

void Acceptor::setReuseAddr(bool on)
{
    int one=on;
    int ret=::setsockopt(_listensock.fd(),SOL_SOCKET,SO_REUSEADDR,&one,sizeof(one));
    if(-1==ret)
    {
        perror("setsockopt");
    }
}

void Acceptor::setReusePort(bool on)
{
    int one=on;
    int ret=::setsockopt(_listensock.fd(),SOL_SOCKET,SO_REUSEPORT,&one,sizeof(one));
    if(-1==ret)
    {
        perror("setsockopt");
    }
}
void Acceptor::bind()
{
    int ret=::bind(_listensock.fd(),(struct sockaddr *)_addr.getInetAdressPtr(),sizeof(struct sockaddr));
    if(-1==ret)
    {
        perror("bind");
    }
}

void Acceptor::listen()
{
    int ret=::listen(_listensock.fd(),10);
    if(-1==ret)
    {
        perror("listen");
    }
}

int Acceptor::accept()
{
    int peerfd=::accept(_listensock.fd(),NULL,NULL);
    if(-1==peerfd)
    {
        perror("accept");
    }
    return peerfd;
}

int Acceptor::fd() const
{
    return _listensock.fd();
}
