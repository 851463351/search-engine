#include "InetAdress.h"

#include <string.h>

InetAdress::InetAdress(unsigned short port)
{
    ::memset(&_addr,0,sizeof(struct sockaddr_in));
    _addr.sin_family=AF_INET;
    _addr.sin_port=::htons(port);
    _addr.sin_addr.s_addr=INADDR_ANY; //代表本机地址
}

InetAdress::InetAdress(const string &ip,unsigned short port)
{
    ::memset(&_addr,0,sizeof(struct sockaddr_in));
    _addr.sin_family=AF_INET;
    _addr.sin_port=::htons(port);
    _addr.sin_addr.s_addr=::inet_addr(ip.c_str());
}

InetAdress::InetAdress(const struct sockaddr_in & addr)
    :_addr(addr) {}

string InetAdress::ip() const
{
    return string(::inet_ntoa(_addr.sin_addr));
}

unsigned short InetAdress::port() const
{
    return ::ntohs(_addr.sin_port);
}

struct sockaddr_in * InetAdress::getInetAdressPtr() 
{
    return &_addr;
}

